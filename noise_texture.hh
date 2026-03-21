#pragma once

#include "hit.hh"
#include "image.hh"
#include "perlin.hh"
#include "ray.hh"
#include "texture_material.hh"

class NoiseTexture : public Texture_Material
{
public:
    Perlin noise;
    double scale; // zoom dans la texture de perlin

    explicit NoiseTexture(const RGB &color, double scale = 1)
        : Texture_Material(color)
        , scale(scale)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;
    RGB getColor() const override
    {
        return color_;
    }
};
