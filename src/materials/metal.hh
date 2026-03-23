#pragma once

#include "material.hh"
#include "texture.hh"

class Metal : public Material
{
public:
    explicit Metal(Texture *texture)
        : texture_(texture)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;

private:
    Texture *texture_;
};
