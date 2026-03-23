#pragma once

#include "hit.hh"
#include "interval.hh"
#include "material.hh"
#include "object.hh"

class HomogeneousVolume : public Object
{
public:
    HomogeneousVolume(Object *boundary, double density, Material *material)
        : Object(material)
        , boundary_(boundary)
        , density_(density)
    {}

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;

private:
    Object *boundary_;
    double density_;
};