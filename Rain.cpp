#include <SDL.h>
#include <random>
#include <time.h> 

#include "SDL_default.h"

int win_w = 800, win_h = 600;

struct WaterDrop
{
	int dx = 8 + double(rand()) / ((double)RAND_MAX / (double)(5 - 1)), dy = 80;
	int movespeed = 20 + rand() % 5;
	SDL_Point drop[2] = { 0, 0 };
	bool uninitialized = true;
};

void WaterDrop_init(WaterDrop& self, int randy, int dx, int dy)
{
	self.uninitialized = false;
	self.dx = dx;
	self.dy = dy;
	self.drop[0] = { rand() % (win_w + 250) - 250, randy * (rand() % win_h) };
	self.drop[1] = { self.drop[0].x - self.dx, self.drop[0].y - self.dy };
}

void WaterDrop_update(WaterDrop& self)
{
	for (SDL_Point& point : self.drop)
	{
		point.x += self.dx * (double(self.movespeed) / self.dy);
		point.y += self.movespeed;
	}
}

void WaterDrop_draw(WaterDrop self)
{
	SDL_SetRenderDrawColor(ren, 184, 215, 255, 255);
	SDL_RenderDrawLines(ren, self.drop, 2);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	init();
	display_init(win_w, win_h);

	const int max_wdrops_count = 100;
	double wdrops_count = 1.0;
	
	WaterDrop wdrops[max_wdrops_count];
	for (int i = 0; i < wdrops_count; i++)
		WaterDrop_init(wdrops[i], 1, rand() % 2 + 5, rand() % 60 + 20 );
	double diff = 0.1;

	SDL_Event ev;
	while (true) 
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit();
			}
		}

		screen_fill(0, 0, 0);
		for (int i = 0; i < wdrops_count; i++)
		{
			if (wdrops[i].uninitialized)
				WaterDrop_init(wdrops[i], 0, rand() % 2 + 5, rand() % 20 + 40);

			WaterDrop_update(wdrops[i]);
			if (wdrops[i].drop[1].y > win_h or wdrops[i].drop[1].x > win_w)
				WaterDrop_init(wdrops[i], 0, rand() % 2 + 5, rand() % 20 + 40);

			WaterDrop_draw(wdrops[i]);
		}

		if (wdrops_count > max_wdrops_count)
			diff = -0.05;
		else if (wdrops_count < 5)
			diff = 0.1;

		wdrops_count += diff;
		

		SDL_RenderPresent(ren);
		SDL_Delay(1000.0 / 60.0);
	}
}