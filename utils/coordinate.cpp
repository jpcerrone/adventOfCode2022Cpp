#include <cstdlib>
#include "coordinate.h"
void Coordinate::operator+=(Coordinate other)
{
    x += other.x;
    y += other.y;
};
void Coordinate::operator-=(Coordinate other)
{
    x -= other.x;
    y -= other.y;
};
bool Coordinate::operator==(Coordinate other)
{
    return ((other.x == x) && (other.y == y));
};
bool Coordinate::operator!=(Coordinate other)
{
    return ((other.x != x) || (other.y != y)); // After migrate to class do !(==)
};
Coordinate Coordinate::operator+(Coordinate other)
{
    Coordinate c = {x + other.x, y + other.y};
    return c;
};
Coordinate Coordinate::operator-(Coordinate other)
{
    Coordinate c = {x - other.x, y - other.y};
    return c;
};
int Coordinate::distance(Coordinate other)
{
    return std::abs(x - other.x) + std::abs(y - other.y);
}
bool Coordinate::inRange(Coordinate lowBound, Coordinate highBound)
{
    return (x >= lowBound.x) && (y >= lowBound.y) && (x <= highBound.x) && (y <= highBound.y);
}