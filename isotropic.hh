#pragma once

#include "perlin.hh"
#include "texture_material.hh"

class Isotropic : public Texture_Material
{
public:
    Perlin perlin;
    double scale;

    explicit Isotropic(const RGB &color, double scale)
        : Texture_Material(color)
        , scale(scale)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &color,
                 Ray &scattered) const override;

    RGB getColor() const override
    {
        return color_;
    }
};