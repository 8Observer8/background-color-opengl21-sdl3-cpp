#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>

#include <iostream>

struct AppContext
{
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_AppResult appQuit = SDL_APP_CONTINUE;
};

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // Init the library, here we make a window so we only need the Video capabilities.
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        return SDL_Fail();
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // Enable MULTISAMPLE
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2); // can be 2, 4, 8 or 16

    // Create a window
    SDL_Window *window = SDL_CreateWindow("SDL3, OpenGL 2.1, C++", 350, 350,
        SDL_WINDOW_OPENGL); // | SDL_WINDOW_RESIZABLE
    if (!window)
    {
        return SDL_Fail();
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        return SDL_Fail();
    }

    if (!gladLoadGL())
    {
        std::cout << "Failed to load OpenGL functions" << std::endl;
        return SDL_APP_FAILURE;
    }

    glClearColor(0.2f, 0.3f, 0.5f, 1.f);

    SDL_ShowWindow(window);

    // Set up the application data
    *appstate = new AppContext {
        window,
        glContext,
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, const SDL_Event *event)
{
    auto *app = (AppContext *)appstate;

    switch (event->type)
    {
        case SDL_EVENT_QUIT:
        {
            app->appQuit = SDL_APP_SUCCESS;
            break;
        }
        default:
        {
            break;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto *app = (AppContext *)appstate;

    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(app->window);

    return app->appQuit;
}

void SDL_AppQuit(void *appstate)
{
    auto *app = (AppContext *)appstate;
    if (app)
    {
        SDL_GL_DestroyContext(app->glContext);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    SDL_Quit();
}
