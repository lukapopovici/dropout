#pragma once
#include <iostream>
class Player : public Entity
{
private:
    bool isInvincible;
    bool isAlive;
    SDL_TimerID timerID;

    Player(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Entity(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {
        this->H = 10;
        this->W = 10;
        this->speed = 3;
        this->isInvincible = false;
        this->isAlive = true;
    }

    ~Player()
    {
        SDL_RemoveTimer(timerID);
    }

    inline static Player *instance = nullptr;

public:
    static void createInstance(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer)
    {
        if (instance == nullptr)
        {
            instance = new Player(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer);
        }
    }

    static void deleteInstance()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }

    static Player *getInstance()
    {
        return instance;
    }

    // functii pentru verificarea coliziunii cu dusmanii

    Uint32 ResetSpeed(Uint32 interval)
    {
        this->speed = 10;
        this->isInvincible = false;
        std::cout << "Speed reset: " << this->speed << std::endl;
        return interval;
    }

    static Uint32 TimerCallback(Uint32 interval, void *param)
    {
        Player *playerInstance = static_cast<Player *>(param);
        playerInstance->ResetSpeed(interval);
        return 0;
    }

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    void Die()
    {
        this->isAlive = false;
        std::cout << "Player died" << std::endl;
    }

    bool IsInvincible()
    {
        return this->isInvincible;
    }

    bool IsAlive()
    {
        return this->isAlive;
    }

    bool OccupiedPossitionCheck()
    {
        int TileNumber = this->GetTile();
        Map *map = Map::GetInstance();
        if (map->tiles[TileNumber].isBlocked)
        {
            return true;
        }
        return false;
    }

    void MoveDown(float deltaTime)
    {
        int originalPosY = this->PosY;
        int changeY = this->PosY + deltaTime * this->speed;
        if (HasCrossedBorders(this->PosX, changeY))
        {
            RespondToBorder();
        }
        else
        {
            this->PosY = changeY;
            if (OccupiedPossitionCheck())
            {
                this->PosY = originalPosY;
            }
        }
    }

    void MoveUp(float deltaTime)
    {
        int originalPosY = this->PosY;
        int changeY = this->PosY - deltaTime * this->speed;
        if (HasCrossedBorders(this->PosX, changeY))
        {
            RespondToBorder();
        }
        else
        {
            this->PosY = changeY;
            if (OccupiedPossitionCheck())
            {
                this->PosY = originalPosY;
            }
        }
    }

    void MoveLeft(float deltaTime)
    {
        int originalPosX = this->PosX;
        int changeX = this->PosX - deltaTime * this->speed;
        if (HasCrossedBorders(changeX, this->PosY))
        {
            RespondToBorder();
        }
        else
        {
            this->PosX = changeX;
            if (OccupiedPossitionCheck())
            {
                this->PosX = originalPosX;
            }
        }
    }

    void MoveRight(float deltaTime)
    {
        int originalPosX = this->PosX;
        int changeX = this->PosX + deltaTime * this->speed;
        if (HasCrossedBorders(changeX, this->PosY))
        {
            RespondToBorder();
        }
        else
        {
            this->PosX = changeX;
            if (OccupiedPossitionCheck())
            {
                this->PosX = originalPosX;
            }
        }
    }

    void ChangePlayerPos(SDL_Keycode code, float deltaTime)
    {
        switch (code)
        {
        case SDLK_w:
            this->MoveUp(deltaTime);
            break;
        case SDLK_s:
            this->MoveDown(deltaTime);
            break;
        case SDLK_a:
            this->MoveLeft(deltaTime);
            break;
        case SDLK_d:
            this->MoveRight(deltaTime);
            break;

        case SDLK_SPACE:
            std::cout << "Current Speed: " << this->speed << std::endl;
            timerID = SDL_AddTimer(2000, TimerCallback, this);

            // pt modificat viteza
            this->speed *= 2;
            this->isInvincible = true;
            break;
        }
    }
};
