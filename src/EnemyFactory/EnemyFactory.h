#pragma once
using namespace std;
class Factory : public Entity
{
private:
    int MAX_ENEMIES = 10;
    int currentEnemies;

public:
    Factory(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Entity(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {
        this->H = 40;
        this->W = 40;
        this->speed = 0; // nu se misca fabrica lol
        currentEnemies = 0;
    }

    void Die()
    {
        this->isAlive = false;
    }

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    string RandomEnemy()
    {
        int random = rand() % 3;
        switch (random)
        {
        case 0:
            return "ShieldEnemy";
            break;
        case 1:
            return "FastEnemy";
            break;
        case 2:
            return "DoublerEnemy";
            break;
        default:
            return "ShieldEnemy";
            break;
        }
    }

    //functie fabrica
    Enemy *CreateEnemy(string enemyType)
    {
        if (currentEnemies < MAX_ENEMIES)
        {
            currentEnemies++;
            std::cout << "Enemy created" << endl;
            if (enemyType == "ShieldEnemy")
            {
                return new Fast(this->BORDER_height, this->BORDER_width, this->PosX + this->W / 2, this->PosY + this->H / 2, this->renderer);
            }
            else if (enemyType == "FastEnemy")
            {
                return new Shield(this->BORDER_height, this->BORDER_width, this->PosX + this->W / 2, this->PosY + this->H / 2, this->renderer);
            }
            else if (enemyType == "DoublerEnemy")
            {
                return new Doubler(this->BORDER_height, this->BORDER_width, this->PosX + this->W / 2, this->PosY + this->H / 2, this->renderer);
            }
        }
        
        return NULL;
    }
};