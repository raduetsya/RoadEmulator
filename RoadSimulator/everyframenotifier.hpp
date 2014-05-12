#ifndef EVERYFRAMENOTIFIER_HPP
#define EVERYFRAMENOTIFIER_HPP

#include "utils.hpp"

class EveryFrameNotifier :
        public Observable<EveryFrameNotifier>
{
public:
    float frameDelay;
	EveryFrameNotifier() :
		frameDelay(0) {}
};

#endif // EVERYFRAMENOTIFIER_HPP
