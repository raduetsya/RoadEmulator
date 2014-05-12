#ifndef SDLCONTROLLER_H
#define SDLCONTROLLER_H

#include "iview.h"
#include "everyframenotifier.h"
#include "keyeventmanager.h"

class SDLController
{
public:
    SDLController();
    ~SDLController();
    void init();
    void gameCycle();

    void addView(IView* view) {
        views.push_back(view);
    }

    KeyEventManager& getKeyListener() {
        return keyMgr;
    }

    EveryFrameNotifier& getFrameNotifier() {
        return frameListener;
    }

private:
    std::list<IView*> views;

    KeyEventManager keyMgr;

    EveryFrameNotifier frameListener;

    SDL_Surface* screen;
};

#endif // SDLCONTROLLER_H
