#pragma once
#include "Buttons.h"

// clasa de baza pentru meniu
// de expandat functionalitate pentru adaugat mai multe butoane
class Menu
{
private:
    SDL_Renderer *renderer;

public:
    Button *button;

    Menu(SDL_Renderer *renderer)
    {
        this->renderer = renderer;
    }
    void Draw()
    {

        button->Draw();
    }
    void CheckInteractions(int PosX, int PosY);
};

// clasa singleton pentru meniul principal
class MainMenu : public Menu
{
private:
    inline static MainMenu *instance = nullptr;
    MainMenu(SDL_Renderer *renderer) : Menu(renderer)
    {
        button = new GameStartButton(200, 450, 150, renderer);
    }

public:
    static void createInstance(SDL_Renderer *renderer)
    {
        if (instance == nullptr)
        {
            instance = new MainMenu(renderer);
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

    static MainMenu *getInstance()
    {
        return instance;
    }

    bool CheckInteractions(int PosX, int PosY)
    {
        if (button->CheckInteractions(PosX, PosY))
        {

            return true;
        }
        return false;
    }
};

// singleton pentru game over screen
class GameOverMenu : public Menu
{
private:
    inline static GameOverMenu *instance = nullptr;
    GameOverMenu(SDL_Renderer *renderer) : Menu(renderer)
    {
        button = new GameOverButton(200, 500, 200, renderer);
    }

public:
    static void createInstance(SDL_Renderer *renderer)
    {
        if (instance == nullptr)
        {
            instance = new GameOverMenu(renderer);
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

    static GameOverMenu *getInstance()
    {
        return instance;
    }

    bool CheckInteractions(int PosX, int PosY)
    {
        if (button->CheckInteractions(PosX, PosY))
        {
            return true;
        }
        return false;
    }
};

class GameWinMenu : public Menu
{
private:
    inline static GameWinMenu *instance = nullptr;
    GameWinMenu(SDL_Renderer *renderer) : Menu(renderer)
    {
        button = new GameWinButton(200, 500, 200, renderer);
    }

public:
    static void createInstance(SDL_Renderer *renderer)
    {
        if (instance == nullptr)
        {
            instance = new GameWinMenu(renderer);
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

    static GameWinMenu *getInstance()
    {
        return instance;
    }

    bool CheckInteractions(int PosX, int PosY)
    {
        if (button->CheckInteractions(PosX, PosY))
        {
            return true;
        }
        return false;
    }
};