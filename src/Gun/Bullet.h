#pragma once
#include <cmath>

class Bullet
{
    int height;
    int width;

    int W = 5;
    int H = 5;
    float angle;
    float speed;
    bool isPresent;
    SDL_Renderer *renderer;

public:
    int x;
    int y;
    Bullet(int arena_height, int arena_width, int x, int y, int mouse_X, int mouse_Y, SDL_Renderer *renderer)
    {
        this->height = height;
        this->width = width;
        this->x = x;
        this->y = y;
        this->angle = atan2(mouse_Y - y, mouse_X - x);
        this->renderer = renderer;
        this->speed = 3;
        this->isPresent = true;
    }
    void Move()
    {
        x = x + speed * cos(angle);
        y = y + speed * sin(angle);
    }
    bool CheckCollision(int arena_height, int arena_width)
    {
        if (x < 0 || x > arena_width || y < 0 || y > arena_height)
        {
            return true;
        }
        return false;
    }
    void Dissaper()
    {
        this->isPresent = false;
    }
    bool IsRenderable()
    {
        return this->isPresent;
    }
    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rect = {x, y, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }
    ~Bullet(){};
};