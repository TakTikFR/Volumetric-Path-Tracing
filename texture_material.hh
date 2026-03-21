#pragma once

#include "hit.hh"
#include "image.hh"
#include "point3.hh"
#include "ray.hh"

class Texture_Material
{
public:
    explicit Texture_Material(const RGB &color)
        : color_(color)
    {}

    virtual ~Texture_Material() = default;

    // virtual Material getMaterial(const Point3& point) const = 0;
    virtual RGB getColor() const = 0;
    virtual bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                         Ray &scattered) const = 0;

protected:
    RGB color_;
};