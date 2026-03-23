#pragma once

#include "material.hh"
#include "texture.hh"

class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture *texture, double intensity = 1.0)
        : texture_(texture)
        , intensity_(intensity)
    {}

    bool scatter(const Ray & /*ray*/, const Hit & /*hit*/,
                 RGB & /*attenuation*/, Ray & /*scattered*/) const override
    {
        return false;
    }

    RGB emitted(const Point3 &point) const override
    {
        RGB c = texture_->value(point);
        return RGB(c.r * intensity_, c.g * intensity_, c.b * intensity_);
    }

private:
    Texture *texture_;
    double intensity_;
};
