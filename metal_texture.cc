#include "metal_texture.hh"

#include "utils.hh"
#include "vector3.hh"

bool MetalTexture::scatter(const Ray &ray, const Hit &hit, RGB &color,
                           Ray &scattered) const
{
    Vector3 scatterDir = reflect(ray.direction, hit.normal);

    if (fabs(scatterDir.x) < 1e-8 || fabs(scatterDir.y) < 1e-8
        || fabs(scatterDir.z) < 1e-8)
        scatterDir = hit.normal;

    scattered = Ray(hit.point, scatterDir);
    color = color_;

    return true;
}