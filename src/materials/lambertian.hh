#pragma once

#include "material.hh"
#include "texture.hh"

class Lambertian : public Material
{
public:
    explicit Lambertian(Texture *texture)
        : texture_(texture)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;

private:
    Texture *texture_;
};
