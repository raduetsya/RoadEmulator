#include "sdlcontroller.h"

#include "config.h"

#include <windows.h>
#include <SDL/SDL.h>
#include <GL/gl.h>

SDLController::SDLController()
{
}

void SDLController::init()
{
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_WM_SetCaption("Road Emulator", NULL);
    screen = SDL_SetVideoMode(
                Config::get().data.SCREEN_WIDTH,
                Config::get().data.SCREEN_HEIGHT,
                32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);

    glViewport(0, 0,
               Config::get().data.SCREEN_WIDTH,
               Config::get().data.SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, Config::get().data.SCREEN_WIDTH,
            Config::get().data.SCREEN_HEIGHT, 0.0,
            0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLoadIdentity();



    glClearColor(1, 1, 1, 1);

}

void SDLController::gameCycle()
{
    SDL_Event event;
    bool quit = false;

    float frameDelay = 1.0f / Config::get().data.SCREEN_MAX_FRAMERATE;
    while (!quit) {
        frameListener.frameDelay = frameDelay;
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = true;
            if (event.type == SDL_KEYDOWN)
                keyMgr.keyEvent(event.key.keysym.sym, true);
            if (event.type == SDL_KEYUP)
                keyMgr.keyEvent(event.key.keysym.sym, false);
        }

        glClear (GL_COLOR_BUFFER_BIT);

        for (auto i = views.begin();
             i != views.end();
             ++i) {
            (*i)->draw();
        }

        frameListener.notify();

        SDL_GL_SwapBuffers( );

        // count fps
        int frameEnd = SDL_GetTicks();
        frameDelay = 0.001f * (frameEnd - frameStart);
        float minDelay = 1.0f/ Config::get().data.SCREEN_MAX_FRAMERATE;
        if ( frameDelay < minDelay ) {
            SDL_Delay( static_cast<Uint32>(1000 * (minDelay - frameDelay)) );
            frameDelay = minDelay;
        }
    }


}

SDLController::~SDLController()
{
    SDL_FreeSurface(screen);
    SDL_Quit();
}
