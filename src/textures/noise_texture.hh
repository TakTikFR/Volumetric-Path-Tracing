#pragma once

#include "perlin.hh"
#include "texture.hh"

class NoiseTexture : public Texture
{
public:
    explicit NoiseTexture(const RGB &color, const Point3 &min_bound,
                          const Point3 &max_bound, double scale = 1.0)
        : color_(color)
        , min_bound_(min_bound)
        , max_bound_(max_bound)
        , scale_(scale)
    {}

    RGB value(const Point3 &point) const override;

private:
    RGB color_;
    Point3 min_bound_;
    Point3 max_bound_;
    double scale_;
    Perlin noise_;
};
