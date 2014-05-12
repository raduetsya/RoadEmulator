#ifndef ROAD_H
#define ROAD_H

#include "car.h"

#include <vector>

#include "utils.hpp"

class RoadType {
public:
    static const int SOLID = 0;
    static const int DASHED = 1;

    static bool isSolid(int type) {
        if (type == SOLID) return true;
        return false;
    }
};

class Road :
        public Observable<Road>
{
public:
    Road()
    {}

    void setLength(int length) {
        parts.resize(length);
        notify();
    }

    void setRoad(int pos, int type) {
        parts.at(pos) = type;
        notify();
    }

    void setIfRightHanded(bool rightHand) {
        this->rightHand = rightHand;
        notify();
    }

    void setWidthHeight(float width, float height) {
        // TODO: do universal
        this->width = width;
        this->height = height;
    }

    int getRoadType(float y) {
        unsigned int pos = static_cast<int>(y / height * parts.size());
        if (pos >= parts.size()) return 0;
        return parts[pos];
    }

    float getSize(char param)
    {
        if (param == 'w') return width;
        else if (param == 'h') return height;
        throw 0;
    }

    bool isRightHanded() { return rightHand; }
    int getLength() { return parts.size(); }
    int getRoad(int pos) { return parts.at(pos); }

    Car& getCar() {
        return car;
    }

private:
    std::vector<int> parts;
    Car car;
    bool rightHand;

    float width, height;

};

#endif // ROAD_H
