#ifndef CAR_H
#define CAR_H

#include "keyeventmanager.h"
#include "utils.h"
#include "everyframenotifier.h"

class Car :
        public Observable<Car>,
        public Observer<KeyEvent>,
        public Observer<EveryFrameNotifier>
{
public:
    Car();
	Car(const Car& other);
    ~Car();

    Car doTick(float delay);
    void update(KeyEvent* ev);
    void update(EveryFrameNotifier *ev);

	void doSpeedUp(bool active = true) {
        _speedup = active;
    }

    void doRotateLeft(bool active = true) {
        _rotLeft = active;
        if (active) _rotRight = false;
    }

    void doRotateRight(bool active = true) {
        _rotRight = active;
        if (active) _rotLeft = false;
    }

    // Left is >0, _angle in (-180:180)
    float getAngle() {
        while (_angle >= 180) _angle -= 360;
        while (_angle < -180) _angle += 360;
        return _angle;
    }

    float getPos(const char d) {
        if (d == 'x') return _x;
        else if (d == 'y') return _y;
        else throw 0;
    }

    void setPos(float x, float y, float angle) {
        _x = x; _y = y;
        _angle = angle;
    }


    Car* getNewState() {
        if (newState == nullptr) newState = new Car();
        return newState;
    }

private:
    bool _speedup;
    bool _rotLeft, _rotRight;

    float _x, _y;
    float _velocity;
    float _angle;

    Car* newState;

	void copyFieldsExceptNewState(const Car* other);
};

#endif // CAR_H
