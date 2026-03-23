#include "metal.hh"

#include <cmath>

#include "hit.hh"
#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

bool Metal::scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                    Ray &scattered) const
{
    Vector3 scatterDir = reflect(ray.direction, hit.normal);

    if (fabs(scatterDir.x) < 1e-8 && fabs(scatterDir.y) < 1e-8
        && fabs(scatterDir.z) < 1e-8)
        scatterDir = hit.normal;

    scattered = Ray(hit.point, scatterDir);
    attenuation = texture_->value(hit.point);

    return true;
}
