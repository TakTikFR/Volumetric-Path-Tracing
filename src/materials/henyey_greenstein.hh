#pragma once

#include "material.hh"
#include "texture.hh"

class HenyeyGreenstein : public Material
{
public:
    explicit HenyeyGreenstein(Texture *texture, double g)
        : texture_(texture)
        , g_(g)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;

private:
    Texture *texture_;
    double g_;
};
