#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "SDL_default.h"
#include "SDL_circle.h"

int win_w = 800, win_h = 600;

struct Snowflake
{
	char randy = 1;
	Circle circle = { double(rand() % (win_w + 250) - 250), double(randy * rand() % win_h), double(rand() % 3 + 1) };
	double movespeed = 1.0 + double(rand()) / ((double)RAND_MAX / (double)(2 - 1));
};

void update_snowflake(Snowflake& snowflake)
{
	snowflake.circle.x += snowflake.movespeed - 1.0;
	snowflake.circle.y += snowflake.movespeed;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	init();
	display_init(win_w, win_h);

	const int snowflakes_n = 200;
	Snowflake snowflakes[snowflakes_n];

	SDL_Event ev;
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
			}
		}

		screen_fill(150, 150, 150);
		for (int i = 0; i < snowflakes_n; i++)
		{
			update_snowflake(snowflakes[i]);
			if (snowflakes[i].circle.y > win_h or snowflakes[i].circle.x > win_w)
				snowflakes[i] = { 0 };

			if (snowflakes[i].circle.x + snowflakes[i].circle.r > 0)
				draw_filled_circle(snowflakes[i].circle, 255, 255, 255);
		}

		SDL_RenderPresent(ren);
		SDL_Delay(1000.0 / 120.0);
	}

	quit();
	return 0;
}