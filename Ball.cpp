#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "SDL_default.h"

int win_w = 800, win_h = 600;

struct Ball
{
	Circle circle;
    double h_velocity = 0.0, v_velocity = 0.0, h_vel_diff = 0.025, v_vel_diff = 0.5;
};

void update_ball(Ball& self, int win_w, int win_h)
{
    win_w--;
    win_h--;

    if (self.h_velocity > 0)
        self.h_velocity -= self.h_vel_diff;
    else if (self.h_velocity)
        self.h_velocity += self.h_vel_diff;

    if (fabs(self.h_velocity) < self.h_vel_diff)
        self.h_velocity = 0;

    self.circle.x += self.h_velocity;

    if (self.circle.x + self.circle.r >= win_w) {
        self.circle.x = win_w - self.circle.r;
        self.h_velocity = -self.h_velocity;
    }
    else if (self.circle.x - self.circle.r <= 0) {
        self.circle.x = self.circle.r;
        self.h_velocity = -self.h_velocity;
    }
                                                          
    self.v_velocity += self.v_vel_diff;
    if (fabs(self.v_velocity) < self.v_vel_diff)
        self.v_velocity = 0;

    
    self.circle.y += self.v_velocity;
    if (self.circle.y + self.circle.r >= win_h) {
        self.circle.y = win_h - self.circle.r;
        self.v_velocity = -self.v_velocity * 0.9;
    }
    else if (self.circle.y - self.circle.r <= 0) {
        self.circle.y = self.circle.r;
        self.v_velocity = -self.v_velocity;
    }
}

void kick_ball(Ball& ball, int mx, int my)
{
    double l_x = ball.circle.x - mx;
    double l_y = ball.circle.y - my;

    double scale = (l_x * l_x + l_y * l_y) / 900.0;
    
    l_x /= scale;
    l_y /= scale;
    
    if (l_x > 20.0)
        l_x = 20.0;
    else if (l_x < -20)
        l_x = -20.0;

    if (l_y > 20.0)
        l_y = 20.0;
    else if (l_y < -20.0)
        l_y = -20.0;

    ball.h_velocity += l_x;
    ball.v_velocity += l_y;
}

int main(int argc, char* argv[])
{
	init();

	display_init(win_w, win_h);

	Ball ball = {{win_w / 2, win_h / 2, 30}};
    
    screen_fill(0, 0, 0);
  
    draw_filled_circle(ball.circle, 200, 0, 0);
    SDL_RenderPresent(ren);
   

    SDL_Delay(700);
    SDL_Event ev;

    bool click = false, is_running = true;
    Uint32 m_state;

    std::cout << "Click to kick the ball" << '\n';

    while (is_running)
    {
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                click = ev.button.button == 1;
            }
        }
        
        if (click)
            kick_ball(ball, ev.button.x, ev.button.y);
        update_ball(ball, win_w, win_h);        
        
        screen_fill(0, 0, 0);
        draw_filled_circle(ball.circle, 200, 0, 0);
        SDL_RenderPresent(ren);
        
        click = false;
        SDL_Delay(1000.0 / 120.0);
    }

    quit(1);
	return 0;
}