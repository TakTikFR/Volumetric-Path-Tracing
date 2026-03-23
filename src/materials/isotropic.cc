#include "isotropic.hh"

#include "hit.hh"
#include "ray.hh"
#include "vector3.hh"

bool Isotropic::scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                        Ray &scattered) const
{
    scattered = Ray(hit.point, Vector3::random_unit_vector(), ray.time);
    attenuation = texture_->value(hit.point);
    return true;
}
