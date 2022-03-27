#pragma once

extern SDL_Window* win;
extern SDL_Renderer* ren;

/* A circle with an origin in the center */
struct Circle {
	double x, y, r;
};

void init(Uint32 flags = SDL_INIT_VIDEO);

void quit(int error = 1);

void display_init(int win_w, int win_h, const char* name = "A window", Uint32 flags = SDL_WINDOW_SHOWN);

void display_quit();

void screen_fill(int r, int g, int b, int a = 255);

void actual_coords(double x, double y, int& x2, int& y2, int centerx, int centery);

/* Circle is a default figure */
void draw_figure(Circle, int r, int g, int b, int a, double angle = 0.0, int points = 30);

void draw_filled_circle(Circle, int r, int g, int b, int a = 255);

SDL_Texture* load_img(const char* file, SDL_Rect& rect);