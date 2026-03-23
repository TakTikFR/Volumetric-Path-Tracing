#pragma once

#include <cmath>

#include "hit.hh"
#include "object.hh"
#include "point3.hh"
#include "texture_material.hh"

class Sphere : public Object
{
public:
    Point3 center;
    double radius;

    Sphere()
        : Object(nullptr)
        , center(0.0, 0.0, 0.0)
        , radius(1.0)
    {}

    Sphere(const Point3 &center, double radius, Texture_Material *material)
        : Object(material)
        , center(center)
        , radius(radius)
    {}

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;
    Vector3 normal(const Point3 &point) const override;
    Texture_Material *getMaterial() const override;
};