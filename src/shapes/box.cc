#include "box.hh"

std::optional<Hit> Box::intersect(const Ray &ray, const interval &ray_int) const
{
    double tMin =
        (min_bound.x - ray.origin.x) / ray.direction.x;
    double tMax =
        (max_bound.x - ray.origin.x) / ray.direction.x;

    if (tMin > tMax)
        std::swap(tMin, tMax);

    double tyMin =
        (min_bound.y - ray.origin.y) / ray.direction.y;
    double tyMax =
        (max_bound.y - ray.origin.y) / ray.direction.y;

    if (tyMin > tyMax)
        std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return std::nullopt;

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

    if (!ray_int.surrounds(tMin))
    {
        tMin = tMax;
        if (!ray_int.surrounds(tMin))
            return std::nullopt;
    }

    Point3 impact = ray.at(tMin);
    Vector3 n = computeNormal(impact);

    Hit hit(impact, n, tMin, material_);
    hit.set_face_normal(ray, n);

    return hit;
}

Vector3 Box::computeNormal(const Point3 &point) const
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
