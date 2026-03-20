#pragma once

#include "texture_material.hh"
#include "hit.hh"

class MetalTexture : public Texture_Material {
public:

    explicit MetalTexture(const RGB& color)
        : Texture_Material(color)
    {}

    bool scatter(const Ray& ray, const Hit& hit, RGB& color, Ray& scattered) const override;
    RGB getColor() const override { return color_; }
};