#include "box.hh"

std::optional<Hit> Box::intersect(const Ray &ray) const
{
    // en gros il y a un couloir volumétrique en X, en Y et en Z
    // si le rayon passe dans les 3 et bien le cube est touché
    double tMin =
        (min_bound.x - ray.origin.x) / ray.direction.x; // entrée couloir X
    double tMax =
        (max_bound.x - ray.origin.x) / ray.direction.x; // sortie couloir X

    if (tMin > tMax)
        std::swap(tMin, tMax);

    double tyMin =
        (min_bound.y - ray.origin.y) / ray.direction.y; // entrée couloir Y
    double tyMax =
        (max_bound.y - ray.origin.y) / ray.direction.y; // sortie couloir Y

    if (tyMin > tyMax)
        std::swap(tyMin, tyMax);

    if ((tMin > tyMax)
        || (tyMin
            > tMax)) // si on a bien été dans le couloir X et Y au même moment
        return std::nullopt;

    // si on est rentré dans le couloir des X avant celui des Y et bien on prend
    // la plus tardive (donc plus proche du cube)
    if (tyMin > tMin)
        tMin = tyMin;
    if (tyMax < tMax)
        tMax = tyMax;

    double tzMin = (min_bound.z - ray.origin.z) / ray.direction.z;
    double tzMax = (max_bound.z - ray.origin.z) / ray.direction.z;

    if (tzMin > tzMax)
        std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return std::nullopt;

    if (tzMin > tMin)
        tMin = tzMin;
    if (tzMax < tMax)
        tMax = tzMax;

    constexpr double tMinLimit = 0.001;
    constexpr double tMaxLimit = 1e30;
    if (tMin < tMinLimit || tMin > tMaxLimit)
    {
        tMin = tMax;
        if (tMin < tMinLimit || tMin > tMaxLimit)
            return std::nullopt;
    }

    Point3 impact = ray.at(tMin);
    Vector3 n = normal(impact);

    Hit hit(impact, n, tMin);
    hit.set_face_normal(ray, n);

    return hit;
}

Vector3 Box::normal(const Point3 &point) const
{
    if (std::abs(point.x - min_bound.x) < 0.001)
        return Vector3(-1, 0, 0);
    if (std::abs(point.x - max_bound.x) < 0.001)
        return Vector3(1, 0, 0);
    if (std::abs(point.y - min_bound.y) < 0.001)
        return Vector3(0, -1, 0);
    if (std::abs(point.y - max_bound.y) < 0.001)
        return Vector3(0, 1, 0);
    if (std::abs(point.z - min_bound.z) < 0.001)
        return Vector3(0, 0, -1);
    if (std::abs(point.z - max_bound.z) < 0.001)
        return Vector3(0, 0, 1);
    return Vector3(0, 1, 0);
}

Texture_Material *Box::getMaterial() const
{
    return textureMaterial_;
}
