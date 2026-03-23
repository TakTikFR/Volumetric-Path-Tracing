#pragma once

#include "perlin.hh"
#include "texture.hh"

class NoiseTexture : public Texture
{
public:
    explicit NoiseTexture(const RGB &color, double scale = 1.0)
        : color_(color)
        , scale_(scale)
    {}

    RGB value(const Point3 &point) const override;

private:
    RGB color_;
    double scale_;
    Perlin noise_;
};
