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

protected:
    Material *material_;
};