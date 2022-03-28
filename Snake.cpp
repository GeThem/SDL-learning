#include <SDL.h>
#include "SDL_default.h"
#include <time.h>
#include <random>
#include <malloc.h>

int win_w = 800, win_h = 600;

struct Snake
{
	int length = 0;
	SDL_Rect* body = NULL;
	Uint8 direction = 0, dir_new = 0;
	int tile_size = 0;
	bool live = false;
};

bool colliderect(SDL_Rect self, SDL_Rect rect)
{
	return self.x == rect.x and self.y == rect.y;
}

int collidelist(SDL_Rect self, SDL_Rect* list, int start, int end)
{
	for (int i = start; i < end; i++)
		if (self.x == list[i].x and self.y == list[i].y)
			return i;
	return -1;
}

void snake_init(Snake& self, int* pg_size, int tile_size = 40, int length = 2)
{
	self.live = true;
	self.length = length;
	self.tile_size = tile_size;
	if (self.body == NULL)
		self.body = (SDL_Rect*)malloc(sizeof(SDL_Rect) * pg_size[0] * pg_size[1]);
	self.direction = self.dir_new =  0;
	if (self.body == NULL)
		quit();
	self.body[0] = { (pg_size[0] / 2) * self.tile_size,  pg_size[1] / 2 * self.tile_size, self.tile_size, self.tile_size };
	self.body[1] = { (pg_size[0] / 2 - 1) * self.tile_size, pg_size[1] / 2 * self.tile_size, self.tile_size, self.tile_size };
}

void snake_move(Snake& self)
{
	if (!self.direction)
		if (self.dir_new == SDL_SCANCODE_RIGHT or self.dir_new == SDL_SCANCODE_UP or self.dir_new == SDL_SCANCODE_DOWN)
			self.direction = self.dir_new;
		else if (self.dir_new == SDL_SCANCODE_LEFT)
			self.direction = SDL_SCANCODE_RIGHT;
	if (self.direction)
	{
		for (int i = self.length - 1; i > 0; i--)
			self.body[i] = self.body[i - 1];

		if (self.dir_new != self.direction)
			if (self.dir_new == SDL_SCANCODE_UP and self.direction == SDL_SCANCODE_DOWN or
				self.dir_new == SDL_SCANCODE_DOWN and self.direction == SDL_SCANCODE_UP or 
				self.dir_new == SDL_SCANCODE_RIGHT and self.direction == SDL_SCANCODE_LEFT or 
				self.dir_new == SDL_SCANCODE_LEFT and self.direction == SDL_SCANCODE_RIGHT)
				self.dir_new = self.direction;

		switch (self.dir_new)
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

		self.direction = self.dir_new;
	}
}

void snake_draw(Snake self)
{
	double diff = 155.0 / self.length;
	for (double i = 0; i < self.length; i++)
	{
		SDL_SetRenderDrawColor(ren, i * diff, int(100 + i * diff), 0, 255);
		SDL_RenderFillRect(ren, &self.body[(int)i]);
	}
}

void apple_draw(SDL_Rect self)
{
	SDL_SetRenderDrawColor(ren, 220, 0, 0, 255);
	SDL_RenderFillRect(ren, &self);
}

SDL_Rect apple_spawn(int* pg_size, int tile_size, Snake snake)
{
	SDL_Rect apple = { rand() % pg_size[0] * tile_size, rand() % pg_size[1] * tile_size, tile_size, tile_size };
	if (collidelist(apple, snake.body, 0, snake.length - 1) != -1)
		apple = { snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, tile_size, tile_size };
	return apple;
}

int main(int argc, char** args)
{
	init();
	display_init(win_w, win_h, "Snake");

	int tile_size = 40;
	int pg_size[] = { win_w / tile_size, win_h / tile_size };
	
	Snake snake;
	snake_init(snake, pg_size, tile_size);


	srand(time(0));
	SDL_Rect apple = apple_spawn(pg_size, tile_size, snake);

	SDL_Event ev;
	const Uint8* kbstate = SDL_GetKeyboardState(NULL);
	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit();
			case SDL_KEYDOWN:
				if (ev.key.keysym.scancode >= SDL_SCANCODE_RIGHT and ev.key.keysym.scancode <= SDL_SCANCODE_UP)
					snake.dir_new = ev.key.keysym.scancode;
				else
					switch (ev.key.keysym.scancode)
					{
					case SDL_SCANCODE_R:
						snake_init(snake, pg_size, tile_size);
						apple = apple_spawn(pg_size, tile_size, snake);
						break;
					case SDL_SCANCODE_ESCAPE:
						snake.live = !snake.live;
						break;
					}
				break;
			}
		}

		screen_fill(0, 0, 0);
		apple_draw(apple);
		snake_draw(snake);
		SDL_RenderPresent(ren);
		

		if (snake.live) 
		{
			if (colliderect(apple, snake.body[0]))
			{
				apple = apple_spawn(pg_size, tile_size, snake);
				snake.length++;
			}

			snake_move(snake);

			if (snake.body[0].x < 0 or snake.body[0].x >= win_w or snake.body[0].y < 0 or snake.body[0].y >= win_h or
				snake.length == pg_size[0] * pg_size[1])
				snake.live = false;

			int collides = collidelist(snake.body[0], snake.body, 3, snake.length);
			if (collides != -1)
				snake.length = collides;

		}
		SDL_Delay(100);
	}
}