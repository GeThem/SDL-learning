#include <SDL.h>
#include <random>
#include <time.h>

#include "SDL_default.h"
#include "SDL_circle.h"

int win_w = 800, win_h = 800;

struct Planet
{
	Circle body;
	Circle orbit;
	double angle_d, cur_angle;
	SDL_Point* points;
	int cur_len, max_points;
	SDL_Color color;
};

void Planet_init(Planet& self, double r, Circle orbit , int trace_points_number, double angle_diff, SDL_Color color)
{
	self.body = {orbit.x + orbit.r, orbit.y, r};
	self.orbit = orbit;
	self.angle_d = angle_diff;
	self.max_points = trace_points_number;
	self.points = (SDL_Point*)malloc(sizeof(SDL_Point) * trace_points_number);
	self.cur_len = 0;
	self.color = color;
}

void Planet_draw(Planet* self)
{
	SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
	SDL_RenderDrawLines(ren, self->points, self->cur_len - 1);
	draw_filled_circle(self->body, self->color.r, self->color.g, self->color.b);
}

void Planet_update(Planet& self)
{
	if (self.cur_len < self.max_points)
		self.cur_len++;
	
	for (int i = self.cur_len - 1; i > 0; i--)
		self.points[i] = self.points[i - 1];

	self.cur_angle += self.angle_d;
	int new_x, new_y;
	actual_coords(self.orbit.r * cos(self.cur_angle / 180.0 * M_PI), self.orbit.r * sin(self.cur_angle / 180.0 * M_PI), new_x, new_y, self.orbit.x, self.orbit.y);
	self.body.x = new_x;
	self.body.y = new_y;
	self.points[0] = { (int)new_x, (int)new_y };
}

int main(int argc, char* argv[])
{
	init();
	display_init(win_w, win_h);

	srand(time(0));
	
	const int star_count = 150;
	SDL_Point stars[star_count];
	for (int i = 0; i < star_count; i++)
		stars[i] = { rand() % win_w, rand() % win_h };

	Circle sun = { win_w / 2, win_h / 2, 60 };
	Planet planets[3];
	Planet_init(planets[0], 20, { double(win_w / 2), double(win_h / 2), 250.0 }, 40, 1.5, { 0, 0, 200 });
	Planet_init(planets[1], 10, { double(win_w / 2), double(win_h / 2), 120.0 }, 20, -1.5, {200, 0, 200});
	Planet_init(planets[2], 15, { double(win_w / 2), double(win_h / 2), 360.0 }, 30, -1, {0, 200, 200});

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
		SDL_SetRenderDrawColor(ren, 250, 250, 250, 255);
		SDL_RenderDrawPoints(ren, stars, star_count);
		draw_filled_circle(sun, 247, 140, 0);
		for (Planet& planet : planets) 
		{
			Planet_draw(&planet);
			Planet_update(planet);
		}

		SDL_RenderPresent(ren);
		SDL_Delay(1000.0 / 60.0);
	}
}