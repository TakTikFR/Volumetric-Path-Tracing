#include "ray.hh"

Point3 Ray::at(double t) const
{
    return { origin.x + t * direction.x, origin.y + t * direction.y,
             origin.z + t * direction.z };
}