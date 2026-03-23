#include "noise_texture.hh"

#include <algorithm>
#include <cmath>

RGB NoiseTexture::value(const Point3 &point) const
{
    double n = 0.5
        * (1.0
           + std::sin(scale_ * point.z
                      + 10.0 * noise_.turbulance(point, 7)));
    n = std::clamp(n, 0.0, 1.0);
    return RGB(color_.r * n, color_.g * n, color_.b * n);
}
