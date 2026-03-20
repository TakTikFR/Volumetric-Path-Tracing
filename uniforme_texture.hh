#pragma once

#include "texture_material.hh"
#include "ray.hh"
#include "image.hh"
#include "hit.hh"

class UniformeTexture : public Texture_Material {
public:

    explicit UniformeTexture(const RGB& color)
        : Texture_Material(color)
    {}

    bool scatter(const Ray& ray, const Hit& hit, RGB& attenuation, Ray& scattered) const override;
    RGB getColor() const override { return color_; }
};