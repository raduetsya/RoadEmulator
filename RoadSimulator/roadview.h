#ifndef ROADVIEW_H
#define ROADVIEW_H

#include "road.h"

#include "lineview.h"

#include "utils.hpp"

class RoadView :
        public LineView,
        public Observer<Road>
{
public:
    RoadView(float left, float top, float right, float bottom)
    {
        setWindow(left,top,right,bottom);
    }

    void update(Road *road) {
        if (road == nullptr) return;

        _vertexArray.resize(2);

        _vertexArray[0] = VertexPair(0, 0, 0, height); // left
        _vertexArray[1] = VertexPair((float)width, 0, (float)width, (float)height); //right

        float pxStep = height / road->getLength();

        for (int i=0; i<road->getLength(); i++) {
            if (road->getRoad(i) == RoadType::SOLID) {
                _vertexArray.push_back( VertexPair(width/2.0f, pxStep*i, width/2.0f, pxStep*(i+1)) );
            }
            if (road->getRoad(i) == RoadType::DASHED) {
                //_vertexArray.push_back( VertexPair(width/2, pxStep*(i+0.2), width/2, pxStep*(i+0.35)) );
                //_vertexArray.push_back( VertexPair(width/2, pxStep*(i+0.6), width/2, pxStep*(i+0.75)) );
            }
        }

        // left arrow
        drawArrow(width/4.0f, height-30.0f, !road->isRightHanded());
        drawArrow(3*width/4.0f, height-30.0f, road->isRightHanded());
    }

    void drawArrow(float x, float y, bool up)
    {
        float arrowSize = 40;
        float pointerDist = 10.0f * (up?1:-1);

        _vertexArray.push_back( VertexPair(x, y-arrowSize/2, x, y+arrowSize/2) );

        y += (arrowSize/2) * (up?-1:1);

        _vertexArray.push_back( VertexPair(x, y, x+pointerDist, y+pointerDist) );
        _vertexArray.push_back( VertexPair(x, y, x-pointerDist, y+pointerDist) );
    }

private:
};

#endif // ROADVIEW_H
