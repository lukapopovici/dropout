#pragma once
#include <vector>
#include "../MobileEntity/EnemyEntity.h"
class Consumable : public EnemyEntity
{
private:
    bool isWhite = true;

public:
    Consumable(int x, int y, int width, int height, SDL_Renderer *renderer) : EnemyEntity(x, y, width, height, renderer)
    {
        this->H = 20;
        this->W = 20;
        this->speed = 0;
    }

    ~Consumable() {}

    void Draw()
    {
        if (isWhite)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            isWhite = false;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            isWhite = true;
        }
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    virtual void NotifyEnemies(std::vector<Entity *> entities) = 0;

    void Die()
    {
        this->isAlive = false;
    }
};

class KillConsumable : public Consumable
{
public:
    KillConsumable(int x, int y, int width, int height, SDL_Renderer *renderer) : Consumable(x, y, width, height, renderer)
    {
    }
    ~KillConsumable() {}

    void NotifyEnemies(std::vector<Entity *> entities)
    {

        for (auto entity : entities)
        {
            if (Enemy *enemy = dynamic_cast<Enemy *>(entity))
            {
                enemy->Die();
                enemy->Die(); // pt inamicii shield
            }
        }
    }
};

class SlowConsumable : public Consumable
{
public:
    SlowConsumable(int x, int y, int width, int height, SDL_Renderer *renderer) : Consumable(x, y, width, height, renderer)
    {
    }
    ~SlowConsumable() {}
    void NotifyEnemies(std::vector<Entity *> entities)
    {

        for (auto entity : entities)
        {
            if (Enemy *enemy = dynamic_cast<Enemy *>(entity))
            {
                enemy->CutSpeed();
            }
        }
    }
};