#ifndef KEYEVENTMANAGER_H
#define KEYEVENTMANAGER_H

#include <SDL/SDL.h>

#include "observer.h"

struct KeyEvent :
        public Observable<KeyEvent>
{
    bool stSpeedUp;
    bool stTurnLeft;
    bool stTurnRight;
	bool stSpaceJustPressed;

    KeyEvent() :
        stSpeedUp(false),
        stTurnLeft(false),
        stTurnRight(false),
		stSpaceJustPressed(false)
    {}

	virtual ~KeyEvent() {};
};

class KeyEventManager
{
public:
    KeyEventManager() {}

	virtual ~KeyEventManager() {}

    template <class T>
    void addListener(Observer<T>& obs) {
        state.attach(obs);
    }

    void keyEvent(SDLKey& key, bool pressed = true) {
		bool needToNotify = false;
        switch (key) {
        case SDLK_UP:
            state.stSpeedUp = pressed;
			needToNotify = true;
            break;
        case SDLK_LEFT:
            state.stTurnLeft = pressed;
			needToNotify = true;
            break;
        case SDLK_RIGHT:
            state.stTurnRight = pressed;
			needToNotify = true;
            break;
		case SDLK_SPACE:
			state.stSpaceJustPressed = pressed;
			needToNotify = pressed;
			break;
        default:
            return;
            break;
        }
        if (needToNotify) state.notify();
		flushOneShotEvents();
    }

	void flushOneShotEvents() {
		state.stSpaceJustPressed = false;
	}

private:
    KeyEvent state;
};

#endif // KEYEVENTMANAGER_H
