#pragma once

#include "material.hh"

class Dielectric : public Material
{
public:
    explicit Dielectric(double refraction_index)
        : refraction_index_(refraction_index)
    {}

    bool scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                 Ray &scattered) const override;

private:
    double refraction_index_;
};
