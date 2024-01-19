#pragma once
#include <typeinfo>
#include "../TileMap/Tiles.h"
#include <iostream>
class Entity
{
protected:
    // variabile generice
    int PosX;
    int PosY;
    int W;
    int H;
    // SPEED
    int speed;
    SDL_Renderer *renderer;
    // variabile dependente de mapa
    int BORDER_width;
    int BORDER_height;
    //
    bool isAlive;

public:
    int GetPosX()
    {
        return this->PosX;
    }

    int GetPosY()
    {
        return this->PosY;
    }

    int GetW()
    {
        return this->W;
    }

    int GetH()
    {
        return this->H;
    }

    bool HasCrossedBorders(int currentX, int currentY)
    {
        int windowWidth = this->BORDER_width;
        int windowHeight = this->BORDER_height;

        if (currentX <= 0 || currentX + this->W >= windowWidth || currentY <= 0 || currentY + this->H >= windowHeight)
        {
            return true;
        }

        return false;
    }

    // nu e nevoie de constructor nul
    Entity(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer)
    {
        this->PosX = StartPosX;
        this->PosY = StartPosY;
        this->renderer = renderer;
        this->BORDER_height = ArenaHeight;
        this->BORDER_width = ArenaWidth;
        this->isAlive = true;
    }

    virtual ~Entity()
    {
    }

    void ChangeSpeed()
    {
        this->speed *= 2;
    }

    virtual void Draw() = 0;
    virtual void Die() = 0;

    bool IsAlive()
    {
        return this->isAlive;
    }

    void RespondToBorder()
    {
        if (typeid(this).name() == "Player")
        {
            return;
        }
        else
        {
            // caz special pentru cand inamicii ajung la border
            return;
        }
    }

    std::pair<int, int> closestNumbers(int x, int y)
    {
        const int step = 25;
        const int seriesStart = 0;

        int closestX = round((x - seriesStart) / static_cast<double>(step)) * step + seriesStart;
        int closestY = round((y - seriesStart) / static_cast<double>(step)) * step + seriesStart;

        return std::make_pair(closestX, closestY);
    }

    // ia numarul tilei in care e entitatea bazat pe x y
    int GetTile()
    {
        int tile = 0;
        std::pair<int, int> coords = this->closestNumbers(this->PosX, this->PosY);

        auto mapInstance = Map::GetInstance();

        auto it = mapInstance->tiles.begin();

        while (it != mapInstance->tiles.end())
        {
            if (it->second.GetCoords() == coords)
            {
                tile = it->first;
                break;
            }

            ++it;
        }
        // std::cout << tile << std::endl;
        return tile;
    }
    // check if Tile is blocked
    bool IsTileBlocked(int tile)
    {
        auto mapInstance = Map::GetInstance();
        return mapInstance->tiles[tile].isBlocked;
    }
};