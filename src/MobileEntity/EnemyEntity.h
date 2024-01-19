#pragma once
#include "Entity.h"
#include "../Player/Player.h"
class EnemyEntity : public Entity
{

public:
    EnemyEntity(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Entity(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {

    }
    bool CheckPlayerCollision()
    {
        if (this->PosX < Player::getInstance()->GetPosX() + Player::getInstance()->GetW() &&
            this->PosX + this->W > Player::getInstance()->GetPosX() &&
            this->PosY < Player::getInstance()->GetPosY() + Player::getInstance()->GetH() &&
            this->PosY + this->H > Player::getInstance()->GetPosY())
        {
            return true;
        }
        return false;
    }
};