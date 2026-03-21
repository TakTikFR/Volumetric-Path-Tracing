#pragma once

#include <cmath>

#include "hit.hh"
#include "object.hh"
#include "point3.hh"
#include "texture_material.hh"

class Box : public Object
{
public:
    Point3 min_bound;
    Point3 max_bound;

    Box()
        : Object(nullptr)
        , min_bound(0.0, 0.0, 0.0)
        , max_bound(1.0, 1.0, 1.0)
    {}

    Box(const Point3 &min_bound, const Point3 &max_bound,
        Texture_Material *material)
        : Object(material)
        , min_bound(min_bound)
        , max_bound(max_bound)
    {}

    std::optional<Hit> intersect(const Ray &ray) const override;
    Vector3 normal(const Point3 &point) const override;
    Texture_Material *getMaterial() const override;
};
