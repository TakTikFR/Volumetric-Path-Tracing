#include "isotropic.hh"
#include "utils.hh"
#include "vector3.hh"

bool Isotropic::scatter(const Ray &ray, const Hit &hit, RGB &color, Ray &scattered) const
{
    scattered = Ray(hit.point, Vector3::random_unit_vector(), ray.time);
    double n = 0.5 * (1.0 + std::sin(scale * hit.point.z + 10.0 * perlin.turbulance(hit.point, 7)));
    uint8_t c = static_cast<uint8_t>(n * 255.0);
    color = RGB(c, c, c);
    return true;
}

