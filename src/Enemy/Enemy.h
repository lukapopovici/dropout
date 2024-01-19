#pragma once
#include <vector>
#include <algorithm>
#include <queue>
#include "../MobileEntity/EnemyEntity.h"
#include "../Gun/Bullet.h"
class Enemy : public EnemyEntity
{

public:
    static std::vector<Entity *> entities2;
    std::vector<std::pair<int, int>> target_node;

    Enemy(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : EnemyEntity(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {
        this->H = 12;
        this->W = 12;
        this->speed = 1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.0)));
    }

    virtual void Die() = 0;
    /*
       bool CheckCollisonWithOtherEnemies(int POSX, int POSY)
       {
           for (Entity *otherEnemy : entities2)
           {

               if (otherEnemy != this)
               {
                   if (POSX < otherEnemy->GetPosX() + otherEnemy->GetW() &&
                       POSX + this->W > otherEnemy->GetPosX() &&
                       POSY < otherEnemy->GetPosY() + otherEnemy->GetH() &&
                       POSY + this->H > otherEnemy->GetPosY())
                   {
                       return false;
                   }
               }
           }
           return true;
       }
   */
    void MoveToTarget(float deltaTime)
    {
        float offset = deltaTime * this->speed;
        if (this->target_node.size() > 0)
        {
            if (this->PosX < this->target_node[0].first)
            {
                this->PosX += offset;
            }
            else if (this->PosX > this->target_node[0].first)
            {
                this->PosX -= offset;
            }
            if (this->PosY < this->target_node[0].second)
            {
                this->PosY += offset;
            }
            else if (this->PosY > this->target_node[0].second)
            {
                this->PosY -= offset;
            }
            else
            {
            }
            if (this->PosX == this->target_node[0].first && this->PosY == this->target_node[0].second)
            {
                this->target_node.erase(this->target_node.begin());
            }
        }
    }

    void AI(float deltaTime)
    {
        this->MoveToTarget(deltaTime);
    }

    void EmptyPath()
    {
        if (this->target_node.size() > 0)
            this->target_node.clear();
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

    //functie pentru observer
    void CutSpeed(){
        this->speed/=2.0;
    }   

    bool CheckCollisionWithBullet(Bullet *bullet)
    {
        if (this->PosX < bullet->x + 5 &&
            this->PosX + this->W > bullet->x &&
            this->PosY < bullet->y +5 &&
            this->PosY + this->H > bullet->y)
        {
            return true;
        }
        return false;
    }

    // functie pentru algoritmul lui Dijsktra

    int heuristic(int tile1, int tile2)
    {
        std::pair<int, int> coords1 = Map::GetInstance()->tiles[tile1].GetCoords();
        std::pair<int, int> coords2 = Map::GetInstance()->tiles[tile2].GetCoords();

        int dx = coords1.first - coords2.first;
        int dy = coords1.second - coords2.second;

        return static_cast<int>(sqrt(dx * dx + dy * dy));
    }

    void PathToPlayer()
    {
        int PlayerTile = Player::getInstance()->GetTile();
        int EnemyTile = this->GetTile();

        std::map<int, std::vector<int>> adjacencyList = Map::GetInstance()->GetAdjacencyList();

        std::map<int, int> distance;
        std::map<int, int> parent;
        std::map<int, bool> visited;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

        for (int i = 0; i < 1600; i++)
        {
            distance[i] = 1000;
            parent[i] = -1;
            visited[i] = false;
        }

        distance[EnemyTile] = 0;
        // mark the blocked tiles as visited

        pq.push(std::make_pair(0 + heuristic(EnemyTile, PlayerTile), EnemyTile));



        while (!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();

            if (visited[u])
            {
                continue;
            }

            visited[u] = true;

            for (int v : adjacencyList[u])
            {
                int weight = 1;

                if (distance[v] > distance[u] + weight)
                {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                    pq.push(std::make_pair(distance[v] + heuristic(v, PlayerTile), v));
                }
            }
        }

        std::vector<int> path;
        int current = PlayerTile;

        while (current != EnemyTile)
        {
            path.push_back(current);
            current = parent[current];
        }

        path.push_back(EnemyTile);

        target_node.clear();

        std::reverse(path.begin(), path.end());

        for (int i = 0; i < path.size(); i++)
        {
            std::pair<int, int> coords = Map::GetInstance()->tiles[path[i]].GetCoords();
            this->target_node.push_back(coords);
        }
    }


};

// diverse clase de inamici

class Doubler : public Enemy
{
public:
    void Die()
    {
        this->isAlive = false;
    }

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 133, 140, 70, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    Doubler(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Enemy(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer) {}


};

class Fast : public Enemy
{
public:
    void Die()
    {
        this->isAlive = false;
    }               

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 133, 200, 70, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    Fast(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Enemy(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {
        this->speed = 2.5 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.0)));
    }


};

class Shield : public Enemy
{
    bool shielded;

public:
    void Die()
    {
        if (shielded)
            shielded = false;
        else if (!shielded)
            this->isAlive = false;
    }

    void Draw()
    {
        SDL_SetRenderDrawColor(renderer, 133, 0, 0, 255);
        SDL_Rect rect = {this->PosX, this->PosY, this->W, this->H};
        SDL_RenderFillRect(renderer, &rect);
    }

    Shield(int ArenaWidth, int ArenaHeight, int StartPosX, int StartPosY, SDL_Renderer *renderer) : Enemy(ArenaWidth, ArenaHeight, StartPosX, StartPosY, renderer)
    {
        shielded = true;
    }


};