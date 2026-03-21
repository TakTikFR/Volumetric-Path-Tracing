#include "dielectric_texture.hh"

#include "utils.hh"
#include "vector3.hh"

bool DielectricTexture::scatter(const Ray &ray, const Hit &hit, RGB &color,
                                Ray &scattered) const
{
    color = RGB(255, 255, 255);
    double ri = hit.front_face ? (1.0 / refraction_index_) : refraction_index_;

    Vector3 unit_direction = ray.direction.normalize();
    Vector3 refracted = refract(unit_direction, hit.normal, ri);

    scattered = Ray(hit.point, refracted);
    return true;
}