#ifndef EVERYFRAMENOTIFIER_H
#define EVERYFRAMENOTIFIER_H

#include "utils.h"

class EveryFrameNotifier :
        public Observable<EveryFrameNotifier>
{
public:
    float frameDelay;
	EveryFrameNotifier() :
		frameDelay(0) {}
};

#endif // EVERYFRAMENOTIFIER_HPP
