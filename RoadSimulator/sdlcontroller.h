#ifndef SDLCONTROLLER_H
#define SDLCONTROLLER_H

#include "iview.h"
#include "everyframenotifier.h"
#include "keyeventmanager.h"

class SDLController
{
public:
	SDLController(std::string windowTitle);
    ~SDLController();
    void init();
    void gameCycle();
	void addView(IView* view);

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
	std::string windowTitle;
	bool inited;
};

#endif // SDLCONTROLLER_H
