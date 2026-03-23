#pragma once

#include "texture.hh"

class SolidColor : public Texture
{
public:
    explicit SolidColor(const RGB &color)
        : color_(color)
    {}

    RGB value(const Point3 & /*point*/) const override
    {
        return color_;
    }

private:
    RGB color_;
};
