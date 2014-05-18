#include "lineview.h"

LineView::~LineView()
{}

void LineView::draw() {
    glLineWidth(1.0f);
    glColor4f (0.0, 0.0, 0.0, 1.0f);
    glBegin(GL_LINES);
    for (unsigned int i=0; i<_vertexArray.size(); ++i) {
        glVertex2f (_vertexArray[i].x1 + window.x1, _vertexArray[i].y1 + window.y1);
        glVertex2f (_vertexArray[i].x2 + window.x1, _vertexArray[i].y2 + window.y1);
    }
    glEnd();
}

void LineView::setWindow(float left, float top, float right, float bottom) {
    window = VertexPair(left, top, right, bottom);
    width = right - left;
    height = bottom - top;
}

void LineView::inheritWindow(LineView& other) {
    window = other.window;
    width = other.width;
    height = other.height;
}


/******
 * LineView::VertexPair implementations
 *
 *****/

LineView::VertexPair::VertexPair() : x1(0), y1(0), x2(0), y2(0) {}

LineView::VertexPair::VertexPair(float fromx, float fromy, float tox, float toy) :
    x1(fromx), y1(fromy),
    x2(tox), y2(toy) {}

LineView::VertexPair::VertexPair(VertexPair& other, float tox, float toy):
    x1(other.x2), y1(other.y2),
    x2(tox), y2(toy) { /* create line from the end of the other line */ }

// rotate counterclockwise, degrees
void LineView::VertexPair::rotate(float degreeAngle) {
	const float pi = 3.1415926f; // should use M_PI here, but cant
    degreeAngle = (-1 * degreeAngle) * pi / 180.0f;
    float nx1 = x1 * cos(degreeAngle) - y1 * sin(degreeAngle);
    float ny1 = x1 * sin(degreeAngle) + y1 * cos(degreeAngle);

    float nx2 = x2 * cos(degreeAngle) - y2 * sin(degreeAngle);
    float ny2 = x2 * sin(degreeAngle) + y2 * cos(degreeAngle);
    x1 = nx1; y1 = ny1;
    x2 = nx2; y2 = ny2;
}

void LineView::VertexPair::translate(float x, float y) {
    x1 += x; y1 += y;
    x2 += x; y2 += y;
}
