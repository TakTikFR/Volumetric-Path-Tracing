#include "noise_texture.hh"

#include <cmath>

#include "vector3.hh"

bool NoiseTexture::scatter(const Ray &ray, const Hit &hit, RGB &color,
                           Ray &scattered) const
{
    Vector3 scatterDir = hit.normal + Vector3::random_unit_vector();

    if (fabs(scatterDir.x) < 1e-8 || fabs(scatterDir.y) < 1e-8
        || fabs(scatterDir.z) < 1e-8)
        scatterDir = hit.normal;

    scattered = Ray(hit.point, scatterDir);
    double n = 0.5
        * (1
           + noise.noise(hit.point
                         * scale)); // récupérer un % de couleur, pas (-1, 1)
    color = RGB(color_.r * n, color_.g * n, color_.b * n);

    return true;
}
