#pragma once

#include "light.hh"
#include "point3.hh"

class PointLight : public Light
{
public:
    Point3 position;

    explicit PointLight(const Point3 &position, double intensity = 1.0,
                        const RGB &color = RGB(255, 255, 255))
        : Light(intensity, color)
        , position(position)
    {}

    Vector3 getDirection(const Point3 &point) const override;
};