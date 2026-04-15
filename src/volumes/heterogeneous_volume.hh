#pragma once

#include "hit.hh"
#include "interval.hh"
#include "material.hh"
#include "object.hh"
#include "texture.hh"

class HeterogeneousVolume : public Object
{
public:
    HeterogeneousVolume(Object *boundary, double density_majorant,
                        Texture *density_texture, Material *material)
        : Object(material)
        , boundary_(boundary)
        , density_majorant_(density_majorant)
        , density_texture_(density_texture)
    {}

    std::optional<Hit> intersect(const Ray &ray,
                                 const interval &ray_int) const override;

private:
    Object *boundary_;
    double density_majorant_;
    Texture *density_texture_;
};