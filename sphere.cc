#include "sphere.hh"

std::optional<Hit> Sphere::intersect(const Ray &ray) const {
  constexpr double tMin = 0.001;
  constexpr double tMax = 1e30;

  Vector3 oc = center - ray.origin;
  double a = ray.direction.dot(ray.direction);
  double h = ray.direction.dot(oc);
  double c = oc.dot(oc) - radius * radius;
  double discriminant = h * h - a * c;

  if (discriminant < 0) {
    return std::nullopt;
  }

  double sqrtd = std::sqrt(discriminant);
  double t = (h - sqrtd) / a;
  if (t <= tMin || tMax <= t) {
    t = (h + sqrtd) / a;

    if (t <= tMin || tMax <= t)
      return std::nullopt;
  }

  Point3 point = ray.at(t);
  Vector3 normal = (point - center) / radius;

  Hit hit(point, normal, t);
  hit.set_face_normal(ray, normal);

  return hit;
}

Vector3 Sphere::normal(const Point3 &point) const {
  return (point - center) / radius;
}

Texture_Material* Sphere::getMaterial() const {
  return textureMaterial_;
}