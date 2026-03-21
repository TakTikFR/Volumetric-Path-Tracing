#include "point_light.hh"

Vector3 PointLight::getDirection(const Point3 &point) const
{
    return (position - point).normalize();
}