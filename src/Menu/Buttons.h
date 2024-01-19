#pragma once

class Button
{
protected:
    int UNIT;
    int width=100;
    int PosX;
    int PosY;
    SDL_Renderer *renderer;

public:
    Button(int unit,int posX, int posY, SDL_Renderer *renderer)
    {
        this->UNIT = unit;
        this->PosX = posX;
        this->PosY = posY;
        this->renderer = renderer;
    }
    
    virtual void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->UNIT, this->width};
        SDL_RenderFillRect(renderer, &rect);
    }

    bool CheckInteractions(int PosX, int PosY)
    {
        if (PosX >= this->PosX && PosX <= this->PosX + this->UNIT && PosY >= this->PosY && PosY <= this->PosY + this->UNIT)
        {
            return true;
        }
        return false;
    }
};

class GameOverButton : public Button
{
public:
    GameOverButton(int unit,int posX, int posY, SDL_Renderer *renderer) : Button(unit,posX, posY, renderer){}
    void Draw()
    {
     
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->UNIT, this->width};
        SDL_RenderFillRect(renderer, &rect);
    } 
};

class GameWinButton : public Button
{
public:
    GameWinButton(int unit,int posX, int posY, SDL_Renderer *renderer) : Button(unit,posX, posY, renderer){}
    void Draw()
    {
   
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->UNIT, this->width};
        SDL_RenderFillRect(renderer, &rect);
    } 
};

class GameStartButton : public Button
{
public:
    GameStartButton(int unit,int posX, int posY, SDL_Renderer *renderer) : Button(unit,posX, posY, renderer){}
    void Draw()
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->UNIT, this->width};
        SDL_RenderFillRect(renderer, &rect);
    } 
};