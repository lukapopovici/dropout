#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
class Map
{
    // Map e singleton
public:
    // reprezentare a hartii pentru construirea listei de adiacenta pt tile-
    int grid[40][40];

    class Tile
    {
    public:
        int width = 24;
        int height = 24;
        int x_coord;
        int y_coord;
        bool isBlocked;
        SDL_Renderer *renderer;

        void Draw()
        {
            // daca e blocat fac un cerc rosu
            if (isBlocked)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                int radius = 9;
                int centerX = this->x_coord + this->width / 2;
                int centerY = this->y_coord + this->height / 2;

                for (int x = -radius; x <= radius; ++x)
                {
                    for (int y = -radius; y <= radius; ++y)
                    {
                        if (x * x + y * y <= radius * radius)
                        {
                            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
                        }
                    }
                }
            }
            // altfel fac un tile albastru
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

                SDL_Rect rect = {this->x_coord, this->y_coord, this->width, this->height};
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        std::pair<int, int> GetCoords()
        {
            return std::pair<int, int>(this->x_coord, this->y_coord);
        }

        Tile()
        {
            this->isBlocked = false;
        }
    };

    int unit = 25;

    std::map<int, Tile> tiles;

    void Draw()
    {
        int iterator = 0;
        for (int i = 0; i < 1000; i += unit)
        {
            for (int j = 0; j < 1000; j += unit)
            {
                Tile tile = tiles[iterator];
                tile.Draw();
                iterator++;
            }
        }
    }

    ~Map() {}

    static void CreateInstance(SDL_Renderer *renderer,int level)
    {
        instance = new Map(renderer,level);
    }

    static Map *GetInstance()
    {
        return instance;
    }

    std::map<int, std::vector<int>> GetAdjacencyList()
    {
        return adjacencyList;
    }

    static void deleteInstance()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        
       

        }
    }

private:
    int level;

    SDL_Renderer *renderer;

    std::map<int, std::vector<int>> adjacencyList;

    // fac lista de adiacenta pe baza matricii
    void BuildAdjacencyList()
    {
        int iterator = 0;
        for (int i = 0; i < 1000; i += unit)
        {
            for (int j = 0; j < 1000; j += unit)
            {
                std::vector<int> neighbours;
                if (grid[(j) / unit][(i) / unit] == 0)
                {
                    if (grid[(j - unit) / unit][(i) / unit] == 0)
                    {
                        neighbours.push_back(iterator - 40);
                    }
                    if (grid[(j + unit) / unit][(i) / unit] == 0)
                    {
                        neighbours.push_back(iterator + 40);
                    }
                    if (grid[(j) / unit][(i - unit) / unit] == 0)
                    {
                        neighbours.push_back(iterator - 1);
                    }
                    if (grid[(j) / unit][(i + unit) / unit] == 0)
                    {
                        neighbours.push_back(iterator + 1);
                    }
                }
                adjacencyList.insert(std::pair<int, std::vector<int>>(iterator, neighbours));
                iterator++;
            }
        }
    }

    // constructorul imi seteaza propietate is blocked pt Draw si
    // face lista de adiacenta

    void ReadGridFromFile(int file_number)
    {
        std::string file_name;

        switch (file_number)
        {
        case 1:
            file_name = "maps/grid1.txt";
            break;
        case 2:
            file_name = "maps/grid2.txt";
            break;
        case 3:
            file_name = "maps/grid3.txt";
            break;
        default:
            file_name = "maps/grid1.txt";
            break;
        }

        std::ifstream file(file_name);

        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file: " + file_name);
        }

        for (int i = 0; i < 40; ++i)
        {
            std::string line;
            std::getline(file, line);
            std::istringstream iss(line);

            for (int j = 0; j < 40; ++j)
            {
                int value;
                if (iss >> value)
                {
                    this->grid[i][j] = value;
                }
                else
                {
                    throw std::runtime_error("Error reading data from file.");
                }
            }
        }

        file.close();
    }

    Map(SDL_Renderer *renderer, int level) : renderer(renderer)
    {
        try
        {
            ReadGridFromFile(level);
        }
        catch (std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        int iterator = 0;
        for (int i = 0; i < 1000; i += unit)
        {
            for (int j = 0; j < 1000; j += unit)
            {
                Tile tile;
                tile.x_coord = i;
                tile.y_coord = j;

                tile.renderer = renderer;
                // setez care tile va fi colorat rosu
                if (grid[(j) / unit][(i) / unit] == 1)

                {
                    tile.isBlocked = true;
                }

                tiles.insert(std::pair<int, Tile>(iterator, tile));
                iterator++;
            }
        }

        BuildAdjacencyList();
    }

    inline static Map *instance = nullptr;
};
