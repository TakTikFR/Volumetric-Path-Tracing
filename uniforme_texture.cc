#include "uniforme_texture.hh"
#include "vector3.hh"
#include <cmath> 

bool UniformeTexture::scatter(const Ray& ray, const Hit& hit, RGB& color, Ray& scattered) const {
  Vector3 scatterDir = hit.normal + Vector3::random_unit_vector();

  if (fabs(scatterDir.x) < 1e-8 || fabs(scatterDir.y) < 1e-8 || fabs(scatterDir.z) < 1e-8)
      scatterDir = hit.normal;

  scattered = Ray(hit.point, scatterDir);
  color = color_;

  return true;
}