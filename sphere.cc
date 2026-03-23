#include "sphere.hh"

std::optional<Hit> Sphere::intersect(const Ray &ray, const interval &ray_int) const
{
    Vector3 oc = center - ray.origin;
    double a = ray.direction.dot(ray.direction);
    double h = ray.direction.dot(oc);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0)
    {
        return std::nullopt;
    }

    double sqrtd = std::sqrt(discriminant);
    double t = (h - sqrtd) / a;
    if (!ray_int.surrounds(t))
    {
        t = (h + sqrtd) / a;

        if (!ray_int.surrounds(t))
            return std::nullopt;
    }

    Point3 point = ray.at(t);
    Vector3 n = normal(point);

    Hit hit(point, n, t);
    hit.set_face_normal(ray, n);

    return hit;
}

Vector3 Sphere::normal(const Point3 &point) const
{
    return (point - center) / radius;
}

Texture_Material *Sphere::getMaterial() const
{
    return textureMaterial_;
}
