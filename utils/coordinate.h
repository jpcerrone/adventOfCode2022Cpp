#pragma once
#include <cstdlib>
struct Coordinate{ // Extract later, migrate to clas
    int x,y;
    void operator +=(Coordinate other);
    void operator -=(Coordinate other);
    bool operator ==(Coordinate other);
    bool operator !=(Coordinate other);
    Coordinate operator +(Coordinate other);
    Coordinate operator -(Coordinate other);
    int distance(Coordinate other);
    bool inRange(Coordinate lowBound, Coordinate highBound);
};