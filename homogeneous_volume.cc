#include "homogeneous_volume.hh"

#include <cmath>

#include "utils.hh"

std::optional<Hit> HomogeneousVolume::intersect(const Ray &ray,
                                                const interval &ray_int) const
{
    std::optional<Hit> hit_in = boundary->intersect(ray, interval::universe);
    if (!hit_in)
        return std::nullopt;

    std::optional<Hit> hit_out =
        boundary->intersect(ray, interval(hit_in->t + 0.0001, infinity));
    if (!hit_out)
        return std::nullopt;

    if (hit_in->t < ray_int.min)
        hit_in->t = ray_int.min;
    if (hit_out->t > ray_int.max)
        hit_out->t = ray_int.max;

    if (hit_in->t >= hit_out->t)
        return std::nullopt;

    if (hit_in->t < 0)
        hit_in->t = 0;

    double ray_length = ray.direction.norm();
    double distance_inside_boundary = (hit_out->t - hit_in->t) * ray_length;
    double hit_distance = -1.0 / density * std::log(random_double());

    if (hit_distance > distance_inside_boundary)
        return std::nullopt;

    double t = hit_in->t + hit_distance / ray_length;
    Point3 point = ray.at(t);
    Vector3 n = Vector3(1, 0, 0); // arbitrary

    Hit hit(point, n, t);
    hit.front_face = true; // also arbitrary

    return hit;
}

Vector3 HomogeneousVolume::normal(const Point3 &point) const
{
    return boundary->normal(point);
}

Texture_Material *HomogeneousVolume::getMaterial() const
{
    return textureMaterial_;
}