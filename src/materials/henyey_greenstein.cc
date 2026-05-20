#include "henyey_greenstein.hh"

#include <cmath>

#include "hit.hh"
#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

// Échantillonne une direction selon la fonction de phase henyey_greenstein
Vector3 hg_sample(const Vector3 direction, double g)
{
    // On échantillonne cos(Theta) selon la distribution HG
    double cosTheta;
    if (std::abs(g) < 1e-3)
        cosTheta = 1.0 - 2.0 * random_double();
    else
    {
        double sqr = (1.0 - g * g) / (1.0 - g + 2.0 * g * random_double());
        cosTheta = (1.0 + g * g - sqr * sqr) / (2.0 * g);
    }

    // Conversion en coordonnées sphériques
    double sin_theta = std::sqrt(std::max(0.0, 1.0 - cosTheta * cosTheta));
    double phi = 2.0 * M_PI * random_double();

    // Construction de la base orthonormée
    Vector3 w = direction.normalize();
    Vector3 helper =
        (std::abs(w.x) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    Vector3 u = w.cross(helper).normalize();
    Vector3 v = w.cross(u);

    // Direction finale
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
