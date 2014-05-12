#ifndef KEYEVENTMANAGER_H
#define KEYEVENTMANAGER_H

#include <SDL/SDL.h>

#include "utils.h"

struct KeyEvent :
        public Observable<KeyEvent>
{
    bool stSpeedUp;
    bool stTurnLeft;
    bool stTurnRight;

    KeyEvent() :
        stSpeedUp(false),
        stTurnLeft(false),
        stTurnRight(false)
    {}
};

class KeyEventManager
{
public:
    KeyEventManager() {
    }

    template <class T>
    void addListener(Observer<T>& obs) {
        state.attach(obs);
    }

    void keyEvent(SDLKey& key, bool pressed = true) {
        switch (key) {
        case SDLK_UP:
            state.stSpeedUp = pressed;
            break;
        case SDLK_LEFT:
            state.stTurnLeft = pressed;
            break;
        case SDLK_RIGHT:
            state.stTurnRight = pressed;
            break;
        default:
            return;
            break;
        }
        state.notify();
    }

private:
    KeyEvent state;
};

#endif // KEYEVENTMANAGER_H
