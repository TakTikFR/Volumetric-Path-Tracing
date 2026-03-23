#pragma once

#include <cmath>

#include "hit.hh"
#include "material.hh"
#include "object.hh"
#include "point3.hh"

class Sphere : public Object
{
public:
    Sphere()
        : Object(nullptr)
        , center(0.0, 0.0, 0.0)
        , radius(1.0)
    {}

    Sphere(const Point3 &center, double radius, Material *material)
        : Object(material)
        , center(center)
        , radius(radius)
    {}

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;

private:
    Point3 center;
    double radius;

    Vector3 computeNormal(const Point3 &point) const;
};