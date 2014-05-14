#include "road.h"

void Road::update(KeyEvent* ev) {
    if (ev == nullptr) return;
	if (ev->stSpaceJustPressed)
		rightHand = !rightHand;
	notify();
}
