#include <SDL.h>
#include "SDL_default.h"
#include "SDL_circle.h"

void draw_figure(Circle circle, int r, int g, int b, int a, double angle, int points)
{
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	int x0, y0;
	actual_coords(circle.r * cos(angle * M_PI / 180.0), circle.r * sin(angle * M_PI / 180.0), x0, y0, circle.x, circle.y);
	double diff = 360.0 / points;
	double fin_angle = 360.0 + angle + diff;
	for (double cur_angle = angle + diff; cur_angle <= fin_angle; cur_angle += diff)
	{
		int x, y;
		actual_coords(circle.r * cos(cur_angle * M_PI / 180.0), circle.r * sin(cur_angle * M_PI / 180.0), x, y, circle.x, circle.y);
		SDL_RenderDrawLine(ren, x0, y0, x, y);
		x0 = x;
		y0 = y;
	}
}

void draw_filled_circle(Circle circle, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	int x1, y1, x2, y2;
	double x0 = -circle.r + 0.25;
	double root = sqrt(circle.r * circle.r - x0 * x0);
	actual_coords(x0, root, x1, y1, circle.x, circle.y);
	actual_coords(x0, -root, x2, y2, circle.x, circle.y);
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	x0 = circle.r - 0.25;
	root = sqrt(circle.r * circle.r - x0 * x0);
	actual_coords(x0, root, x1, y1, circle.x, circle.y);
	actual_coords(x0, -root, x2, y2, circle.x, circle.y);
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);

	for (double x = -circle.r; x <= circle.r; x++)
	{
		root = sqrt(circle.r * circle.r - x * x);
		actual_coords(x, root, x1, y1, circle.x, circle.y);
		actual_coords(x, -root, x2, y2, circle.x, circle.y);
		SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}
}