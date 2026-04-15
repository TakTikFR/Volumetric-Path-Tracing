#include "noise_texture.hh"

#include <algorithm>
#include <cmath>

#include "noise_texture.hh"

// Petite helper function pour smoothstep fade
double smoothstep(double edge0, double edge1, double x)
{
    double t = std::clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

// Fonction de bruit vectoriel FBM (Nécessaire pour le Warping)
Vector3 vector_fbm(const Perlin &noise, const Point3 &p, int octaves)
{
    // On sample le FBM standard sur 3 axes avec de gros offsets pour découpler
    return Vector3(noise.fbm(p, octaves),
                   noise.fbm(p + Point3(5.2, 1.3, 2.8), octaves),
                   noise.fbm(p + Point3(10.4, 2.6, 5.6), octaves));
}
// RGB NoiseTexture::value(const Point3 &point) const
// {
//     // ─── 1. Calcul de l'enveloppe (Forme de Sphère / Ellipse)
//     // ───────────────── On convertit les coordonnées pour qu'elles aillent
//     de
//     // -1.0 à 1.0 (0.0 étant le centre)
//     double nx =
//         2.0 * (point.x - min_bound_.x) / (max_bound_.x - min_bound_.x) - 1.0;
//     double ny =
//         2.0 * (point.y - min_bound_.y) / (max_bound_.y - min_bound_.y) - 1.0;
//     double nz =
//         2.0 * (point.z - min_bound_.z) / (max_bound_.z - min_bound_.z) - 1.0;
//
//     // Calcul de la distance au centre (d = 0 au centre, d > 1 dans les coins
//     de
//     // la boîte)
//     double d = std::sqrt(nx * nx + ny * ny + nz * nz);
//
//     // Fade radial : La densité est de 100% jusqu'à 40% du volume, puis tombe
//     à
//     // 0% vers les bords
//     double fade = smoothstep(1.0, 0.4, d);
//
//     // ─── 2. Domain Warping Récursif (Technique d'Iñigo Quilez) ───────────
//     Vector3 warp1 = vector_fbm(noise_, point * scale_ * 0.5, 4);
//     Point3 warped_p1 = point + warp1 * 2.0;
//
//     Vector3 warp2 = vector_fbm(noise_, warped_p1 * scale_ * 1.5, 4);
//     Point3 deeply_warped_p = warped_p1 + warp2 * 1.0;
//
//     // ─── 3. Calcul Densité Finale ───────────────
//     double n = 0.5
//         * (1.0
//            + std::sin(scale_ * deeply_warped_p.z
//                       + 10.0 * noise_.fbm(deeply_warped_p, 7)));
//
//     // On applique notre sphère douce (fade) sur le bruit
//     double final_n = n * fade;
//
//     final_n = std::clamp(final_n, 0.0, 1.0);
//     return RGB(color_.r * final_n, color_.g * final_n, color_.b * final_n);
// }

RGB NoiseTexture::value(const Point3 &point) const
{
    // ─── 1. Calcul de l'enveloppe (Forme de Fumigène) ─────────────
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

    // 🚀 L'OPTIMISATION ULTIME : Empty Space Skipping 🚀
    // Si on est en dehors du cône de fumée, on renvoie du vide IMMEDIATEMENT.
    // Cela sauve des milliards de calculs de Perlin Noise !
    if (fade < 0.001)
    {
        return RGB(0, 0, 0);
    }

    // ─── 2. Domain Warping Récursif (Allégé) ───────────
    // On passe de 4 octaves à 2 octaves !
    Vector3 warp1 = vector_fbm(noise_, point * scale_ * 0.5, 2);
    Point3 warped_p1 = point + warp1 * 2.0;

    // On passe de 4 octaves à 2 octaves !
    Vector3 warp2 = vector_fbm(noise_, warped_p1 * scale_ * 1.5, 2);
    Point3 deeply_warped_p = warped_p1 + warp2 * 1.0;

    // ─── 3. Calcul Densité Finale ───────────────
    // On passe de 7 octaves à 3 octaves !
    double n = 0.5
        * (1.0
           + std::sin(scale_ * deeply_warped_p.z
                      + 10.0 * noise_.fbm(deeply_warped_p, 3)));

    double final_n = n * fade;

    final_n = std::clamp(final_n, 0.0, 1.0);
    return RGB(color_.r * final_n, color_.g * final_n, color_.b * final_n);
}
