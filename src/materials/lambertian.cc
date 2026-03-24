#include "lambertian.hh"

#include <cmath>

#include "hit.hh"
#include "ray.hh"
#include "vector3.hh"

bool Lambertian::scatter(const Ray & /*ray*/, const Hit &hit, RGB &attenuation,
                         Ray &scattered) const
{
    Vector3 scatterDir = hit.normal + Vector3::random_unit_vector();

    if (fabs(scatterDir.x) < 1e-8 && fabs(scatterDir.y) < 1e-8
        && fabs(scatterDir.z) < 1e-8)
        scatterDir = hit.normal;

    scattered = Ray(hit.point, scatterDir);
    attenuation = texture_->value(hit.point);

    return true;
}
