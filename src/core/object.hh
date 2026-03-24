#pragma once

#include <optional>

#include "hit.hh"
#include "interval.hh"
#include "ray.hh"

class Object
{
public:
    explicit Object(Material *material = nullptr)
        : material_(material)
    {}

    virtual ~Object() = default;

    virtual std::optional<Hit> intersect(const Ray &ray,
                                         const interval &ray_int) const = 0;
    Material* getMaterial() const { return material_; }
    virtual Vector3 random(const Point3& origin) const {
        return Vector3(1, 0, 0);
    }
    virtual double pdf_value(const Point3& origin, const Vector3& direction) const {
        return 0.0;
    }

protected:
    Material *material_;
};
