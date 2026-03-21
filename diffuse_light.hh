#pragma once

#include "texture_material.hh"

class DiffuseLight : public Texture_Material
{
public:
    double intensity;

    explicit DiffuseLight(const RGB &color, double intensity = 1.0)
        : Texture_Material(color)
        , intensity(intensity)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override
    {
        return false;
    };
    RGB getColor() const override
    {
        return color_;
    }
    RGB emitted(const Point3 & /* p */) const override
    {
        return RGB(color_.r * intensity, color_.g * intensity,
                   color_.b * intensity);
    }
};
