#ifndef CARVIEW_H
#define CARVIEW_H

#include "car.h"
#include "lineview.h"
#include "utils.h"

class CarView :
        public LineView,
        public Observer<Car>
{
public:
    CarView(float size, LineView* parent = nullptr);
    void update(Car *ev);

private:
    float size;

	void normalize();
};

#endif // CARVIEW_H
