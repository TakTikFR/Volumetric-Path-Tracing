#pragma once

#include "image.hh"
#include "point3.hh"
#include "vector3.hh"

class Light
{
public:
    double intensity;
    RGB color;

    Light(double intensity = 1.0, const RGB &color = RGB(255, 255, 255))
        : intensity(intensity)
        , color(color)
    {}

    virtual ~Light() = default;

    virtual Vector3 getDirection(const Point3 &point) const = 0;
};