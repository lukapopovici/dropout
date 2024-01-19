#pragma once

enum StateName
{
    GameState_N,
    GamePause_N,
    GameMenu_N,
    GameOver_N,
    GameWin_N
};

class State
{

public:
    StateName currentSTATE;
    StateName nextSTATE;
    virtual void Render(float deltaTime) = 0;
    virtual void Update(StateName state) = 0;
    virtual void GatherInput(SDL_Event event, float deltatime) = 0;
};

class GameState : public State
{
private:
public:
    inline static bool firstTime = true;
    void Render(float deltaTime)
    {
        Arena::GetInstance()->MapDraw();
        Arena::GetInstance()->DrawPlayer();

        Arena::GetInstance()->MoveEnemies(deltaTime);
        Arena::GetInstance()->DrawEnemies();
        //
        Arena::GetInstance()->MoveProjectiles();
        Arena::GetInstance()->DrawProjectiles();
        //
        Arena::GetInstance()->CheckEntities();

        if (!Player::getInstance()->IsAlive())
        {
            Update(GameOver_N);
        }
        if (Arena::GetInstance()->ReturnEntitiesSize() == 0)
        {
            SDL_Delay(1000);

            Update(GameWin_N);
        }
    }

    void Update(StateName state)
    {
        this->nextSTATE = state;
    }

    void GatherInput(SDL_Event event, float deltaTime)
    {

        switch (event.type)
        {
        case SDL_KEYDOWN:

        {
            if (event.key.keysym.sym == SDLK_p)
            {
                Update(GamePause_N);
                Arena::GetInstance()->isPaused = true;
            }
            else
                Arena::GetInstance()->MovePlayer(event.key.keysym.sym, deltaTime);
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Arena::GetInstance()->SpawnBullet(event.button.x, event.button.y);
            }
            break;
        }
        // Arena::GetInstance()->CopyVectorIntoEnemy();
    };
    GameState(SDL_Renderer *renderer)
    {
        this->currentSTATE = GameState_N;
        this->nextSTATE = GameState_N;
        if (firstTime)
        {
            Arena::CreateInstance(renderer);
            Arena::GetInstance()->CreatePlayer();
            Arena::GetInstance()->CreateFactory();

            Arena::GetInstance()->StartSpawn();
        }
        GameOverMenu::deleteInstance();
        MainMenu::deleteInstance();
        firstTime = false;
    }
};

class GamePause : public State
{

public:
    void Render(float deltaTime)
    {
        Arena::GetInstance()->MapDraw();
        Arena::GetInstance()->DrawPlayer();
        Arena::GetInstance()->DrawEnemies();
        Arena::GetInstance()->DrawProjectiles();
    }

    void Update(StateName state)
    {
        this->nextSTATE = state;
    }

    void GatherInput(SDL_Event event, float deltatime)
    {

        switch (event.type)
        {

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_p)
            {
                Update(GameState_N);
                Arena::GetInstance()->isPaused = false;
            }
            break;
        }
    }

    GamePause(SDL_Renderer *renderer)
    {
        this->currentSTATE = GamePause_N;
        this->nextSTATE = GamePause_N;
    }
};

class GameMenu : public State
{
public:
    void Render(float deltaTime)
    {
        MainMenu::getInstance()->Draw();
    };
    void Update(StateName state)
    {
        this->nextSTATE = state;
    };
    void GatherInput(SDL_Event event, float deltatime)
    {
        switch (event.type)
        {

        case SDL_MOUSEBUTTONDOWN:
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (MainMenu::getInstance()->CheckInteractions(mouseX, mouseY))
                Update(GameState_N);

            break;
        }
    }
    GameMenu(SDL_Renderer *renderer)
    {
        this->currentSTATE = GameMenu_N;
        this->nextSTATE = GameMenu_N;
        Arena::deleteInstance();
        MainMenu::createInstance(renderer);
    }
};

class GameOver : public State
{
public:
    void Render(float deltaTime)
    {
        GameOverMenu::getInstance()->Draw();
    };

    void Update(StateName state)
    {
        this->nextSTATE = state;
    };

    void GatherInput(SDL_Event event, float deltatime)
    {
        switch (event.type)
        {

        case SDL_MOUSEBUTTONDOWN:
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (GameOverMenu::getInstance()->CheckInteractions(mouseX, mouseY))
                Update(GameMenu_N);

            break;
        }
    }
    GameOver(SDL_Renderer *renderer)
    {
        this->currentSTATE = GameOver_N;
        this->nextSTATE = GameOver_N;
        Arena::deleteInstance();
        GameOverMenu::createInstance(renderer);
        GameState::firstTime = true;
    }
};

class GameWin : public State
{
public:
    void Render(float deltaTime)
    {
        GameWinMenu::getInstance()->Draw();
    };

    void Update(StateName state)
    {
        this->nextSTATE = state;
    };

    void GatherInput(SDL_Event event, float deltatime)
    {
        switch (event.type)
        {

        case SDL_MOUSEBUTTONDOWN:
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (GameWinMenu::getInstance()->CheckInteractions(mouseX, mouseY))
                Update(GameMenu_N);

            break;
        }
    }
    GameWin(SDL_Renderer *renderer)
    {
        this->currentSTATE = GameWin_N;
        this->nextSTATE = GameWin_N;
        Arena::deleteInstance();
        GameWinMenu::createInstance(renderer);
        GameState::firstTime = true;
    }
};

void SetState(State *&state, SDL_Renderer *renderer, StateName stateName)
{
    switch (stateName)
    {
    case GameState_N:
        state = new GameState(renderer);
        break;
    case GamePause_N:
        state = new GamePause(renderer);
        break;
    case GameMenu_N:
        state = new GameMenu(renderer);
        break;
    case GameOver_N:
        state = new GameOver(renderer);
        break;
    case GameWin_N:
        state = new GameWin(renderer);
        break;
    }
}

void ChangeState(State *&state, SDL_Renderer *renderer)
{
    if (state->nextSTATE != state->currentSTATE)
    {
        state->currentSTATE = state->nextSTATE;
        SetState(state, renderer, state->currentSTATE);
    }
    else
    {
        return;
    }
}