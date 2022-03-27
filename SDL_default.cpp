#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "SDL_default.h"

SDL_Window* win;
SDL_Renderer* ren;

void init(Uint32 flags)
{
	if (SDL_Init(flags)) {
		printf_s("Coudn't init SDL! Error: %s", SDL_GetError());
		system("pause");
		quit(1);
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		printf_s("Coudn't init IMG! Error: %s", SDL_GetError());
		system("pause");
		quit(1);
	}
}

void quit(int error)
{
	if (ren != NULL)
		SDL_DestroyRenderer(ren);
	if (win != NULL)
		SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
	exit(error);
}

void display_init(int win_w, int win_h, const char* name, Uint32 flags) {
	win = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, flags);
	if (win == NULL) {
		printf_s("Couldn't create window! Error: %s", SDL_GetError());
		system("pause");
		quit(1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) {
		printf_s("Couldn't create renderer! Error: %s", SDL_GetError());
		system("pause");
		quit(1);
	}
}

void display_quit() {
	if (ren != NULL)
		SDL_DestroyRenderer(ren);
	if (win != NULL)
		SDL_DestroyWindow(win);
}

void screen_fill(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	SDL_RenderClear(ren);
}

void actual_coords(double x, double y, int& x2,int& y2, int centerx, int centery)
{
	x2 = lround(centerx + x);
	y2 = lround(centery - y);
}

void draw_figure(Circle circle, int r, int g, int b, int a,double angle, int points)
{
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	int x0, y0;
	actual_coords(circle.r * cos(angle * M_PI / 180.0), circle.r * sin(angle * M_PI / 180.0), x0, y0, circle.x, circle.y);
	double diff = 360.0 / points;
	double fin_angle = 360.0 + angle + diff;
	for (double cur_angle = angle + diff; cur_angle <= fin_angle; cur_angle += diff)
	{
		int x, y;
		actual_coords(circle.r * cos(cur_angle * M_PI / 180.0), circle.r* sin(cur_angle * M_PI / 180.0), x, y, circle.x, circle.y);
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

SDL_Texture* load_img(const char* file, SDL_Rect& rect)
{
	SDL_Surface* surface = IMG_Load("Z:\\My folder\\Screens\\147.jpg");
	if (surface == NULL)
	{
		printf("Coudln't load image %s! Error: %s", file, SDL_GetError());
		quit(1);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	rect = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);
	return texture;
}
