#pragma once

#include "point3.hh"
#include "vector3.hh"

class Ray
{
public:
    Point3 origin;
    Vector3 direction;
    double time;

    Ray() = default;

    Ray(const Point3 &origin, const Vector3 &direction)
        : origin(origin)
        , direction(direction)
        , time(0)
    {}

    Ray(const Point3 &origin, const Vector3 &direction, double time)
        : origin(origin)
        , direction(direction)
        , time(time)
    {}

    Point3 at(double t) const;
};