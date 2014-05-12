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
    CarView(float size, LineView* parent = nullptr) :
        size(size)
    {
        if (parent != nullptr) {
            inheritWindow(*parent);
        }

        _vertexArray.resize(5);
    }

    void update(Car *ev) {
        if (ev == nullptr) {
            normalize();
            return;
        }

        Car* car = ev;

        normalize();

        for (int i=0; i<5; ++i) {
            _vertexArray[i].rotate(car->getAngle());
            _vertexArray[i].translate(car->getPos('x'), car->getPos('y'));
        }
    }


private:

    void normalize() {
        _vertexArray[0] = VertexPair(0-size, 0-size, 0-size, 0+size);  // left
        _vertexArray[1] = VertexPair(_vertexArray[0], 0+size, 0+size); // bottom
        _vertexArray[2] = VertexPair(_vertexArray[1], 0+size, 0-size); // right
        _vertexArray[3] = VertexPair(_vertexArray[2], 0-size, 0-size); // top

        _vertexArray[4] = VertexPair(0, 0, 0, 0-size*2); // forward
    }

    float size;
};

#endif // CARVIEW_H
