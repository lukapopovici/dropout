#pragma once
#include <vector>
#include <mutex>
#include <thread>
#include "../TileMap/Tiles.h"
#include "../EnemyFactory/EnemyFactory.h"
#include "../Consumables/Consumable.h"
#include "../Gun/Bullet.h"
using namespace std;
class Arena
{
private:
   int level = 1;
   int height = 1000;
   int width = 1000;
   SDL_Renderer *renderer;

   // timere
   SDL_TimerID timerID;

   inline static Arena *instance = nullptr;

   Map *map = 0;

   std::vector<Entity *> entities;

   std::vector<Bullet *> projectiles;

   inline static std::mutex timerMutex;

   Arena(SDL_Renderer *renderer)
   {
      this->height = height;
      this->width = width;
      this->renderer = renderer;
      Map::CreateInstance(renderer, level);
      this->map = Map::GetInstance();
      this->isPaused = false;
   }

   ~Arena()
   {
      SDL_RemoveTimer(timerID);

      for (Entity *entity : entities)
      {
         delete entity;
      }
      entities.clear();
      projectiles.clear();
      Map::deleteInstance();
      Player::deleteInstance();
   }

   int maxFactories = 2;
   int currentFactories = 0;

public:
   bool isPaused;

   static void CreateInstance(SDL_Renderer *renderer)
   {
      if (!instance)
      {
         instance = new Arena(renderer);
      }
      else
      {
         return;
      }
   }

   static void IncreaseLevel()
   {
      instance->level++;
   }

   static void CreateMap()
   {
      Map::CreateInstance(instance->renderer, instance->level);
      instance->map = Map::GetInstance();
   }
   void DeleteMap()
   {
      Map::deleteInstance();
      if (instance)
      {
         instance->currentFactories = 0;
      }
   }

   static void deleteInstance()
   {
      if (instance)
      {
         delete instance;
         instance = nullptr;
      }
   }

   static Arena *GetInstance()
   {
      return instance;
   }

   void CreatePlayer()
   {
      Player::createInstance(this->width, this->height, this->width / 2, this->height / 2, renderer);
   }

   void DrawPlayer()
   {
      Player::getInstance()->Draw();
   }

   void MovePlayer(SDL_Keycode code, float deltaTime)
   {
      if (Player::getInstance())
         Player::getInstance()->ChangePlayerPos(code, deltaTime);
      else
      {
         return;
      }
   }
   void DeletePlayer()
   {
      Player::deleteInstance();
   }
   int ReturnEntitiesSize()
   {
      return this->entities.size();
   }
   void CreateFactory()
   {
      currentFactories++;
      if (currentFactories > maxFactories)
         return;
      else
      {
         // configurarea nivelelor
         // fiecare nivel e mai greu nu doar din pricina hartii, ci si din faptul ca pot fi mai multi inamici generati de catre fabrici
         if (this->level == 1)
         {
            this->entities.push_back(new Factory(this->width, this->height, 130, 130, renderer, 5));
            this->entities.push_back(new Factory(this->width, this->height, 630, 630, renderer, 5));
            this->entities.push_back(new KillConsumable(this->width, this->height, 750, 750, renderer));
         }
         else if (this->level == 2)
         {
            this->entities.push_back(new Factory(this->width, this->height, 830, 830, renderer, 3));
            this->entities.push_back(new Factory(this->width, this->height, 430, 430, renderer, 3));
            this->entities.push_back(new KillConsumable(this->width, this->height, 250, 250, renderer));
         }
         else if (this->level == 3)
         {
            this->entities.push_back(new Factory(this->width, this->height, 630, 630, renderer, 10));
            this->entities.push_back(new SlowConsumable(this->width, this->height, 750, 750, renderer));
         }
      }
   }

   void SpawnBullet(int mouse_x, int mouse_y)
   {
      Bullet *bullet = new Bullet(this->width, this->height, Player::getInstance()->GetPosX(), Player::getInstance()->GetPosY(), mouse_x, mouse_y, renderer);
      this->projectiles.push_back(bullet);
   }

   bool CheckPlayer()
   {
      if (Player::getInstance())
         return true;
      else
         return false;
   }

   void ClearProjectiles()
   {
      if (this->projectiles.size() == 0)
         return;
      else
      {
         for (int i = 0; i < this->projectiles.size(); i++)
         {
            delete this->projectiles[i];
         }
         this->projectiles.clear();
      }
   }

   static Uint32 TimerCallback(Uint32 interval, void *param)
   {
      std::lock_guard<std::mutex> lock(timerMutex);
      // verific mutexul inainte de rulare
      if (!timerMutex.try_lock())
      {
         Arena *ARENA = static_cast<Arena *>(param);
         ARENA->FactorySpawnEnemies();
         return interval;
      }
      else
      {
         return interval;
      }
   }

   void MapDraw()
   {
      map->Draw();
   }

   void StartSpawn()
   {
      std::lock_guard<std::mutex> lock(timerMutex);
      timerID = SDL_AddTimer(500, TimerCallback, this);
   }
   // functie care itereaza prin fabricile din vectorul meu de entitati inamice
   // si le face sa scuipe dusmani
   void FactorySpawnEnemies()
   {
      for (int i = 0; i < entities.size(); i++)
      {
         if (this->isPaused)
         {
            return;
         }
         else
         {
            if (Factory *factory = dynamic_cast<Factory *>(entities[i]))
            {
               Enemy *enemy = factory->CreateEnemy(factory->RandomEnemy());

               if (enemy)
               {
                  entities.push_back(enemy);
               }
               else
               {
                  factory->Die();
                  return;
               }
            }
         }
      }
      UpdateEnemyAI();
   }

   // functie timer #2
   void UpdateEnemyAI()
   {
      if (entities.size() == 0)
      {
         return;
      }
      else
      {
         for (int i = 0; i < entities.size(); i++)
         {
            if (Enemy *enemy = dynamic_cast<Enemy *>(entities[i]))
            {
               enemy->PathToPlayer();
            }
         }
      }
   }

   void MoveProjectiles()
   {
      for (int i = 0; i < this->projectiles.size(); i++)
      {
         this->projectiles[i]->Move();
         if (this->projectiles[i]->CheckCollision(this->height, this->width))
         {
            this->projectiles[i]->Dissaper();
         }
      }
   }
   void DrawProjectiles()
   {
      for (int i = 0; i < this->projectiles.size(); i++)
      {
         if (this->projectiles[i]->IsRenderable())
            this->projectiles[i]->Draw();
      }
   }
   // functie care updateaza pozitia inamicilor si verifica coliziunea cu
   // Playerul
   void CheckCollison(Enemy *enemy)
   {
      if (enemy->CheckPlayerCollision())
      {
         std::cout << "Player collided with enemy" << std::endl;
         if (Player::getInstance()->IsInvincible())
         {
            enemy->Die();
         }
         else
         {
            Player::getInstance()->Die();
         }
      }
      if (projectiles.size() == 0)
         return;
      else
      {
         for (int i = 0; i < this->projectiles.size(); i++)
         {
            if (enemy->CheckCollisionWithBullet(this->projectiles[i]))
            {
               enemy->Die();
               this->projectiles[i]->Dissaper();
            }
         }
      }
   }
   void MoveEnemies(float deltaTime)
   {
      for (int i = 0; i < this->entities.size(); i++)
      {
         if (Enemy *enemy = dynamic_cast<Enemy *>(this->entities[i]))
         {
            enemy->AI(deltaTime);
            CheckCollison(enemy);
         }
         else if (Consumable *consumable = dynamic_cast<Consumable *>(this->entities[i]))
         {
            if (consumable->CheckPlayerCollision())
            {
               consumable->NotifyEnemies(this->entities);
               consumable->Die();
            }
         }
      }
   }

   void DrawEnemies()
   {
      for (int i = 0; i < this->entities.size(); i++)
      {
         this->entities[i]->Draw();
      }
   }

   // scap de inamicii morti
   void CheckEntities()
   {
      if (this->entities.size() == 0)
      {
      }
      else
      {
         for (int i = 0; i < this->entities.size(); i++)
         {
            if (!this->entities[i]->IsAlive())
            {
               // chem destructorul sa ma fereasca Dumnezeu de memory leak
               delete this->entities[i];
               this->entities.erase(this->entities.begin() + i);
            }
         }
      }
      if (this->projectiles.size() == 0)
      {
      }
      else
      {
         for (int i = 0; i < this->projectiles.size(); i++)
         {
            if (!this->projectiles[i]->IsRenderable())
            {
               delete this->projectiles[i];
               this->projectiles.erase(this->projectiles.begin() + i);
            }
         }
      }
   }
};