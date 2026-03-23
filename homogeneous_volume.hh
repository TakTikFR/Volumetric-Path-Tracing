#pragma once

#include "hit.hh"
#include "interval.hh"
#include "object.hh"
#include "sphere.hh"

class HomogeneousVolume : public Object
{
public:
    Object *boundary;
    double density;

    HomogeneousVolume(Object *boundary, double density,
                      Texture_Material *material)
        : Object(material)
        , boundary(boundary)
        , density(density)
    {}

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;
    Vector3 normal(const Point3 &point) const override;
    Texture_Material *getMaterial() const override;
};