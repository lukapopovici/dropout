#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#define WIDTH 1000
#define HEIGHT 1000

class Game {
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    State* tate = nullptr;

public:
    Game() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "Failed to initialize SDL2\n";
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        window = SDL_CreateWindow("#DROPOUT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Failed to create window\n";
            return;
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "Failed to create OpenGL context\n";
            return;
        }

        // Initialize GLEW (or GLAD)
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW\n";
            return;
        }

        glViewport(0, 0, WIDTH, HEIGHT);
        glEnable(GL_DEPTH_TEST);  // Optional: for 3D

        SetState(tate, window, GameMenu_N);  // Adjust to pass window if needed
    }

    void Run() {
        SDL_Event event;
        bool quit = false;
        Uint32 start = SDL_GetTicks();
        float deltaTime = 0.1f;

        while (!quit) {
            ChangeState(tate, window);  // May want to pass OpenGL-specific stuff instead

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    quit = true;

                try {
                    tate->GatherInput(event, deltaTime);
                } catch (const std::exception& e) {
                    std::cerr << e.what() << '\n';
                }
            }

            // Clear screen using OpenGL
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Call render function (should now use OpenGL internally)
            tate->Render(deltaTime);

            SDL_GL_SwapWindow(window);

            Uint32 endTime = SDL_GetTicks();
            deltaTime = (endTime - start) / 1000.0f;
            start = endTime;

            SDL_Delay(10);
        }

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};
