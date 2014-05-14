#include "car.h"
#include "config.h"

#include <cmath>

Car::Car() :
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

Car::Car(const Car& other) :
	newState(nullptr)
{
	this->copyFieldsExceptNewState(&other);
}

Car::~Car() {
    if (newState != nullptr) delete newState;
}


Car Car::doTick(float delay) {
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

	const float pi = 3.1415926f; // should use cmath::M_PI here
    float radAngle = (-res._angle) * pi / 180.0f;
    float dx = sin(radAngle) * res._velocity;
    float dy = -cos(radAngle) * res._velocity;
    res._x += dx * delay;
    res._y += dy * delay;

    return res;
}

void Car::update(KeyEvent* ev) {
    if (ev == nullptr) return;
    doSpeedUp(ev->stSpeedUp);
    doRotateLeft(ev->stTurnLeft);
    doRotateRight(ev->stTurnRight);
}

void Car::update(EveryFrameNotifier *ev)
{
    Car tempState(doTick(ev->frameDelay));
    getNewState();
	newState->copyFieldsExceptNewState(&tempState);
    notify();
	this->copyFieldsExceptNewState(newState);
}

void Car::copyFieldsExceptNewState(const Car* other) {
	this->_speedup = other->_speedup;
	this->_x = other->_x;
	this->_y = other->_y;
	this->_angle = other->_angle;
	this->_velocity = other->_velocity;
	this->_rotLeft = other->_rotLeft;
	this->_rotRight = other->_rotRight;
	//this->newState = nullptr;
}
