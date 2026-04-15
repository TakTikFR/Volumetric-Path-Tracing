#pragma once

#include <cmath>

#include "hit.hh"
#include "interval.hh"
#include "material.hh"
#include "object.hh"
#include "point3.hh"

class Box : public Object
{
public:
    Box()
        : Object(nullptr)
        , min_bound(0.0, 0.0, 0.0)
        , max_bound(1.0, 1.0, 1.0)
    {}

    Box(const Point3 &min_bound, const Point3 &max_bound, Material *material)
        : Object(material)
        , min_bound(min_bound)
        , max_bound(max_bound)
    {}

    Point3 getMinBound() const
    {
        return min_bound;
    };
    Point3 getMaxBound() const
    {
        return max_bound;
    };

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;

    Vector3 computeNormal(const Point3 &point) const;
    Vector3 random(const Point3 &origin) const override;
    double pdf_value(const Point3 &origin,
                     const Vector3 &direction) const override;

private:
    Point3 min_bound;
    Point3 max_bound;
};
