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
      Map::CreateInstance(renderer);
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
         Factory *factory = new Factory(this->width, this->height, 130, 130, renderer);
         this->entities.push_back(factory);
         Factory *factory2 = new Factory(this->width, this->height, 630, 630, renderer);
         this->entities.push_back(factory2);
         KillConsumable *consumable = new KillConsumable(this->width, this->height, 750, 750, renderer);
         this->entities.push_back(consumable);
      }
   }

   void SpawnBullet(int mouse_x, int mouse_y)
   {
      Bullet *bullet = new Bullet(this->width, this->height, Player::getInstance()->GetPosX(), Player::getInstance()->GetPosY(), mouse_x, mouse_y, renderer);
      this->projectiles.push_back(bullet);
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
   // Plang cand comentez asta
   /*
      void CopyVectorIntoEnemy()
      {
         Enemy::entities2.clear();
         //std::cout << "Vector cleared" << std::endl;
         for (int i = 0; i < entities.size(); i++)
         {
            if (Enemy *enemy = dynamic_cast<Enemy *>(entities[i]))

            {
                  Enemy::entities2.push_back(enemy);
               //   std::cout << "Enemy added to vector" << std::endl;
            }
         }
      }
      */
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
      for (int i = 0; i < this->entities.size(); i++)
      {
         if (Enemy *enemy = dynamic_cast<Enemy *>(this->entities[i]))
         {
            enemy->PathToPlayer();
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
      for (int j = 0; j < this->projectiles.size(); j++)
      {
         if (enemy->CheckCollisionWithBullet(this->projectiles[j]))
         {
            enemy->Die();
            this->projectiles[j]->Dissaper();
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
      for (int i = 0; i < this->entities.size(); i++)
      {
         if (!this->entities[i]->IsAlive())
         {
            // chem destructorul sa ma fereasca Dumnezeu de memory leak
            delete this->entities[i];
            this->entities.erase(this->entities.begin() + i);
         }
      }
      for (int i = 0; i < this->projectiles.size(); i++)
      {
         if (!this->projectiles[i]->IsRenderable())
         {
            delete this->projectiles[i];
            this->projectiles.erase(this->projectiles.begin() + i);
         }
      }
   }
};