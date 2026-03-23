#pragma once

#include "image.hh"
#include "point3.hh"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual RGB value(const Point3 &point) const = 0;
};
