#ifndef CAR_H
#define CAR_H

#include "config.h"

#include "keyeventmanager.h"
#include "utils.h"
#include "everyframenotifier.h"

#define _USE_MATH_DEFINES
#include <cmath>

class Car :
        public Observable<Car>,
        public Observer<KeyEvent>,
        public Observer<EveryFrameNotifier>
{
public:
    Car() :
        _speedup(false),
        _rotLeft(false),
        _rotRight(false),
		_angle(0),
        _velocity(0),
		_x(0),
		_y(0),
		newState(nullptr)
    {
	}

	Car(const Car& other) :
		newState(nullptr)
	{
		this->copyFieldsExceptNewState(&other);
	}

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

    // Left is >0, (-180:180)
    float getAngle() {
        while (_angle >= 180)
            _angle -= 360;
        while (_angle < -180)
            _angle += 360;
        return _angle;
    }

    float getPos(const char d) {
        if (d == 'x')
            return _x;
        else if (d == 'y')
            return _y;
        else
            throw 0;
    }

    void setPos(float x, float y, float angle) {
        _x = x; _y = y;
        _angle = angle;
    }

    Car doTick(float delay) {
        Car res(*this);

        if (_rotLeft)
            res._angle += Config::get().data.CAR_TURN_SPEED * delay;

        if (_rotRight)
            res._angle -= Config::get().data.CAR_TURN_SPEED * delay;

        if (_speedup)
            res._velocity += Config::get().data.CAR_ACCELERATE_SPEED * delay;
        else
            res._velocity -= Config::get().data.CAR_ACCELERATE_SPEED * delay;

        if (res._velocity < 0) res._velocity = 0;

        if (res._velocity > Config::get().data.CAR_MAX_SPEED)
            res._velocity = Config::get().data.CAR_MAX_SPEED;

		const float pi = 3.1415926f; // should use M_PI here
        float radAngle = (-res._angle) * pi / 180.0f;
        float dx = sin(radAngle) * res._velocity;
        float dy = -cos(radAngle) * res._velocity;
        res._x += dx * delay;
        res._y += dy * delay;

        return res;
    }

    void update(KeyEvent* ev) {
        if (ev == nullptr) return;
        doSpeedUp(ev->stSpeedUp);
        doRotateLeft(ev->stTurnLeft);
        doRotateRight(ev->stTurnRight);
    }

    void update(EveryFrameNotifier *ev)
    {
        Car tempState(doTick(ev->frameDelay));
        getNewState();
		newState->copyFieldsExceptNewState(&tempState);
        notify();
		this->copyFieldsExceptNewState(newState);
    }

    Car* getNewState() {
        if (newState == nullptr) {
            newState = new Car();
        }
        return newState;
    }

    ~Car() {
        if (newState != nullptr) {
            delete newState;
        }
    }

private:
    bool _speedup;
    bool _rotLeft, _rotRight;

    float _x, _y;
    float _velocity;
    float _angle;

    Car* newState;

	void copyFieldsExceptNewState(const Car* other) {
		this->_speedup = other->_speedup;
		this->_x = other->_x;
		this->_y = other->_y;
		this->_angle = other->_angle;
		this->_velocity = other->_velocity;
		this->_rotLeft = other->_rotLeft;
		this->_rotRight = other->_rotRight;
		//this->newState = nullptr;
	}
};

#endif // CAR_H
