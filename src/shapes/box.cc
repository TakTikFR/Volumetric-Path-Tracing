#include "box.hh"

#include <algorithm>

#include "utils.hh"

std::optional<Hit> Box::intersect(const Ray &ray, const interval &ray_int) const
{
    double tMin_x, tMax_x, tMin_y, tMax_y, tMin_z, tMax_z;

    if (std::abs(ray.direction.x) < 1e-8)
    {
        if (ray.origin.x < min_bound.x || ray.origin.x > max_bound.x)
            return std::nullopt;
        tMin_x = -1e10;
        tMax_x = 1e10;
    }
    else
    {
        tMin_x = (min_bound.x - ray.origin.x) / ray.direction.x;
        tMax_x = (max_bound.x - ray.origin.x) / ray.direction.x;
        if (tMin_x > tMax_x)
            std::swap(tMin_x, tMax_x);
    }

    if (std::abs(ray.direction.y) < 1e-8)
    {
        if (ray.origin.y < min_bound.y || ray.origin.y > max_bound.y)
            return std::nullopt;
        tMin_y = -1e10;
        tMax_y = 1e10;
    }
    else
    {
        tMin_y = (min_bound.y - ray.origin.y) / ray.direction.y;
        tMax_y = (max_bound.y - ray.origin.y) / ray.direction.y;
        if (tMin_y > tMax_y)
            std::swap(tMin_y, tMax_y);
    }

    if (std::abs(ray.direction.z) < 1e-8)
    {
        if (ray.origin.z < min_bound.z || ray.origin.z > max_bound.z)
            return std::nullopt;
        tMin_z = -1e10;
        tMax_z = 1e10;
    }
    else
    {
        tMin_z = (min_bound.z - ray.origin.z) / ray.direction.z;
        tMax_z = (max_bound.z - ray.origin.z) / ray.direction.z;
        if (tMin_z > tMax_z)
            std::swap(tMin_z, tMax_z);
    }

    double tMin = std::max({ tMin_x, tMin_y, tMin_z });
    double tMax = std::min({ tMax_x, tMax_y, tMax_z });

    if (tMin > tMax)
        return std::nullopt;

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
Vector3 Box::random(const Point3 &origin) const
{
    // point random face du bas
    Point3 random_point;
    random_point.x = random_double(min_bound.x, max_bound.x);
    random_point.y = min_bound.y;
    random_point.z = random_double(min_bound.z, max_bound.z);

    return random_point - origin;
}

double Box::pdf_value(const Point3 &origin, const Vector3 &direction) const
{
    // proba d'avoir tiré ce rayon vu qu'on force l'aléatoire (pour pas détruire
    // la moyenne)
    std::optional<Hit> rec =
        this->intersect(Ray(origin, direction), interval(0.001, infinity));

    // si on rate la boite ou touche le dessous
    if (!rec || std::abs(rec->normal.y + 1.0) > 1e-3)
        return 0;

    // surface de la face du bas
    double area_y_bottom =
        (max_bound.x - min_bound.x) * (max_bound.z - min_bound.z);

    double distance_squared = rec->t * rec->t * direction.dot(direction);
    double cosine = std::fabs(direction.normalize().dot(rec->normal));

    if (cosine < 1e-8)
        return 0;

    return distance_squared / (cosine * area_y_bottom);
}
