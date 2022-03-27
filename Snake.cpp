#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "SDL_default.h"

int win_w = 800, win_h = 600;

struct Snake
{
	int length = 0;
	SDL_Rect body[300] = {};
	Uint8 direction = 0, moves = 0;
	int tile_size = 0;
};

void snake_init(Snake& self)
{
	self.length = 3;
	self.tile_size = 40;
	if (self.body == NULL)
	{
		printf_s("Snake coudln't initialize");
		quit(0);
	}
	self.body[0] = { 11 * self.tile_size, 7 * self.tile_size, self.tile_size, self.tile_size };
	self.body[1] = { 10 * self.tile_size, 7 * self.tile_size, self.tile_size, self.tile_size };
	self.body[2] = { 9 * self.tile_size, 7 * self.tile_size, self.tile_size, self.tile_size };
}

void snake_move(Snake& self)
{
	if (!self.direction)
		if (self.moves == SDL_SCANCODE_RIGHT or self.moves == SDL_SCANCODE_UP or self.moves == SDL_SCANCODE_DOWN)
			self.direction = self.moves;
		else if (self.moves == SDL_SCANCODE_LEFT)
			self.direction = SDL_SCANCODE_RIGHT;
	if (self.direction)
	{
		for (int i = self.length; i > 0; i--)
			self.body[i] = self.body[i - 1];

		if (self.moves != self.direction)
			if (self.moves == SDL_SCANCODE_UP and self.direction == SDL_SCANCODE_DOWN or
				self.moves == SDL_SCANCODE_DOWN and self.direction == SDL_SCANCODE_UP or 
				self.moves == SDL_SCANCODE_RIGHT and self.direction == SDL_SCANCODE_LEFT or 
				self.moves == SDL_SCANCODE_LEFT and self.direction == SDL_SCANCODE_RIGHT)
				self.moves = self.direction;

		switch (self.moves)
		{
		case SDL_SCANCODE_UP:
			self.body[0].y -= self.tile_size;
			break;
		case SDL_SCANCODE_DOWN:
			self.body[0].y += self.tile_size;
			break;
		case SDL_SCANCODE_RIGHT:
			self.body[0].x += self.tile_size;
			break;
		case SDL_SCANCODE_LEFT:
			self.body[0].x -= self.tile_size;
			break;
		}

		self.direction = self.moves;
	}
}

int snake_collide_self(Snake self)
{
	for (int i = 3; i < self.length; i++)
		if (self.body[i].x == self.body[0].x and self.body[i].y == self.body[0].y)
			return i;
	return 0;
}

void snake_draw(Snake self)
{
	SDL_SetRenderDrawColor(ren, 0, 220, 0, 255);
	SDL_RenderFillRects(ren, self.body, self.length);
}

void apple_draw(SDL_Rect self)
{
	SDL_SetRenderDrawColor(ren, 220, 0, 0, 255);
	SDL_RenderFillRect(ren, &self);
}

SDL_Rect apple_spawn(int* pg_size, int tile_size)
{
	return { rand() % pg_size[0] * tile_size, rand() % pg_size[1] * tile_size, tile_size, tile_size };
}

bool colliderect(SDL_Rect self, SDL_Rect rect)
{
	return self.x == rect.x and self.y == rect.y;
}

int main(int argc, char** args)
{
	init();
	display_init(win_w, win_h);

	Snake snake;
	snake_init(snake);

	int pg_size[] = { win_w / snake.tile_size - 1, win_h / snake.tile_size - 1};
	srand(time(0));
	SDL_Rect apple = apple_spawn(pg_size, snake.tile_size);

	SDL_Event ev;
	const Uint8* kbstate = SDL_GetKeyboardState(NULL);
	bool is_running = true;
	while (is_running)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				is_running = false;
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.scancode >= SDL_SCANCODE_RIGHT and ev.key.keysym.scancode <= SDL_SCANCODE_UP)
					snake.moves = ev.key.keysym.scancode;
				break;
			}
		}

		screen_fill(0, 0, 0);
		apple_draw(apple);
		snake_draw(snake);
		SDL_RenderPresent(ren);
		
		if (colliderect(apple, snake.body[0]))
		{
			apple = apple_spawn(pg_size, snake.tile_size);
			snake.length++;
		}

		snake_move(snake);

		if (snake.body[0].x < 0 or snake.body[0].x >= win_w or snake.body[0].y < 0 or snake.body[0].y >= win_h)
			is_running = false;

		int collides = snake_collide_self(snake);
		if (collides)
			snake.length = collides;

		SDL_Delay(100);
	}
	quit();
	return 0;
}