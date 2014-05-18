#ifndef EVERYFRAMENOTIFIER_H
#define EVERYFRAMENOTIFIER_H

#include "observer.h"

class EveryFrameNotifier :
        public Observable<EveryFrameNotifier>
{
public:
    float frameDelay;
	EveryFrameNotifier() :
		frameDelay(0) {}

	virtual ~EveryFrameNotifier() {};
};

#endif // EVERYFRAMENOTIFIER_HPP
