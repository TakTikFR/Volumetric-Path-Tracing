#pragma once

#include "texture_material.hh"
#include "hit.hh"

class DielectricTexture : public Texture_Material {
public:

    explicit DielectricTexture(const RGB& color, double refraction_index)
        : Texture_Material(color)
        , refraction_index_(refraction_index)
    {}

    bool scatter(const Ray& ray, const Hit& hit, RGB& color, Ray& scattered) const override;
    RGB getColor() const override { return color_; }
    double getRefractionIndex() const { return refraction_index_; }

private:
    double refraction_index_;
};

