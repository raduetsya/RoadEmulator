#ifndef ROADVIEW_H
#define ROADVIEW_H

#include "road.h"
#include "lineview.h"
#include "observer.h"

class RoadView :
        public LineView,
        public Observer<Road>
{
public:
    RoadView(float left, float top, float right, float bottom);
	virtual ~RoadView();
    void update(Road *road);
private:
    void drawArrow(float x, float y, bool up);
};

#endif // ROADVIEW_H
