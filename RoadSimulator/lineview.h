#ifndef LINEVIEW_H
#define LINEVIEW_H

#include "iview.h"
#include "utils.h"

#include <windows.h>
#include <GL/gl.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

class LineView : public IView
{
public:
    void draw();

protected:
    struct VertexPair {
        GLfloat x1, y1, x2, y2;

        VertexPair();
        VertexPair(float fromx, float fromy, float tox, float toy);
        VertexPair(VertexPair& other, float tox, float toy);

        // rotate counterclockwise, degrees
        void rotate(float degreeAngle);
        void translate(float x, float y);
    };

    std::vector<VertexPair> _vertexArray;

    float width, height;

private:
    // window.(x1,y1) on screen === (0,0) on _vertexArray
    // window.(x2,y2) is right-bottom
    VertexPair window;

public:
    void setWindow(float left, float top, float right, float bottom);

protected:
    void inheritWindow(LineView& other);
};


#endif // LINEVIEW_H
