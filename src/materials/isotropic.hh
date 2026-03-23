#pragma once

#include "material.hh"
#include "texture.hh"

class Isotropic : public Material
{
public:
    explicit Isotropic(Texture *texture)
        : texture_(texture)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;

private:
    Texture *texture_;
};