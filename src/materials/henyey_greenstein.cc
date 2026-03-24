#include "henyey_greenstein.hh"

#include <cmath>

#include "hit.hh"
#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

double pdf_value(const Vector3 &wo, const Vector3 &wi, double g)
{
    double cosTheta = wo.normalize().dot(wi.normalize());
    return (1.0 - g * g)
        / (4.0 * M_PI * std::pow(1.0 + g * g - 2.0 * g * cosTheta, 1.5));
}

Vector3 hg_sample(const Vector3 direction, double g)
{
    double cosTheta;
    if (std::abs(g) < 1e-3)
        cosTheta = 1.0 - 2.0 * random_double();
    else
    {
        double sqr = (1.0 - g * g) / (1.0 - g + 2.0 * g * random_double());
        cosTheta = (1. - 0 + g * g - sqr * sqr) / (2.0 * g);
    }

    double sin_theta = std::sqrt(std::max(0.0, 1.0 - cosTheta * cosTheta));
    double phi = 2.0 * M_PI * random_double();

    Vector3 w = direction.normalize();
    Vector3 helper =
        (std::abs(w.x) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    Vector3 u = w.cross(helper).normalize();
    Vector3 v = w.cross(u);

    return u * sin_theta * std::cos(phi) + v * sin_theta * std::sin(phi)
        + w * cosTheta;
}

bool HenyeyGreenstein::scatter(const Ray &ray, const Hit &hit, RGB &attenuation,
                               Ray &scattered) const
{
    Vector3 dir = hg_sample(ray.direction, g_);
    scattered = Ray(hit.point, dir, ray.time);
    attenuation = texture_->value(hit.point);
    return true;
}
