#include "noise_texture.hh"

#include <algorithm>
#include <cmath>

#include "noise_texture.hh"

double smoothstep(double edge0, double edge1, double x)
{
    double t = std::clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

Vector3 vector_fbm(const Perlin &noise, const Point3 &p, int octaves)
{
    return Vector3(noise.fbm(p, octaves),
                   noise.fbm(p + Point3(5.2, 1.3, 2.8), octaves),
                   noise.fbm(p + Point3(10.4, 2.6, 5.6), octaves));
}
// RGB NoiseTexture::value(const Point3 &point) const
// {
//     double nx =
//         2.0 * (point.x - min_bound_.x) / (max_bound_.x - min_bound_.x) - 1.0;
//     double ny =
//         2.0 * (point.y - min_bound_.y) / (max_bound_.y - min_bound_.y) - 1.0;
//     double nz =
//         2.0 * (point.z - min_bound_.z) / (max_bound_.z - min_bound_.z) - 1.0;
//
//     // distance du centre
//     double d = std::sqrt(nx * nx + ny * ny + nz * nz);
//
//     double fade = smoothstep(1.0, 0.4, d);
//
//     Vector3 warp1 = vector_fbm(noise_, point * scale_ * 0.5, 4);
//     Point3 warped_p1 = point + warp1 * 2.0;
//
//     Vector3 warp2 = vector_fbm(noise_, warped_p1 * scale_ * 1.5, 4);
//     Point3 deeply_warped_p = warped_p1 + warp2 * 1.0;
//
//     double n = 0.5
//         * (1.0
//            + std::sin(scale_ * deeply_warped_p.z
//                       + 10.0 * noise_.fbm(deeply_warped_p, 7)));
//
//     double final_n = n * fade;
//
//     final_n = std::clamp(final_n, 0.0, 1.0);
//     return RGB(color_.r * final_n, color_.g * final_n, color_.b * final_n);
// }

RGB NoiseTexture::value(const Point3 &point) const
{
    double u = (point.x - min_bound_.x) / (max_bound_.x - min_bound_.x);
    double v = (point.y - min_bound_.y) / (max_bound_.y - min_bound_.y);
    double w = (point.z - min_bound_.z) / (max_bound_.z - min_bound_.z);

    double dist_xz =
        std::sqrt(std::pow(u - 0.5, 2) + std::pow(w - 0.5, 2)) * 2.0;
    double radius_at_height = 0.1 + 0.9 * v;

    double horizontal_fade =
        smoothstep(radius_at_height, radius_at_height * 0.4, dist_xz);
    double vertical_fade = smoothstep(1.0, 0.4, v);
    double fade = horizontal_fade * vertical_fade;

    if (fade < 0.001)
        return RGB(0, 0, 0);

    Vector3 warp1 = vector_fbm(noise_, point * scale_ * 0.5, 2);
    Point3 warped_p1 = point + warp1 * 2.0;

    Vector3 warp2 = vector_fbm(noise_, warped_p1 * scale_ * 1.5, 2);
    Point3 deeply_warped_p = warped_p1 + warp2 * 1.0;

    double n = 0.5
        * (1.0
           + std::sin(scale_ * deeply_warped_p.z
                      + 10.0 * noise_.fbm(deeply_warped_p, 3)));

    double final_n = n * fade;

    final_n = std::clamp(final_n, 0.0, 1.0);
    return RGB(color_.r * final_n, color_.g * final_n, color_.b * final_n);
}
