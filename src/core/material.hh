#pragma once

#include "image.hh"
#include "point3.hh"

class Ray;
class Hit;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                         Ray &scattered) const = 0;

    virtual RGB emitted(const Point3 & /*point*/) const
    {
        return RGB(0, 0, 0);
    }
};
