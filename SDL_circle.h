#pragma once

/* A circle with an origin in the center */
struct Circle {
	double x, y, r;
};

/* Circle is a default figure */
void draw_figure(Circle, int r, int g, int b, int a, double angle = 0.0, int points = 30);

void draw_filled_circle(Circle, int r, int g, int b, int a = 255);