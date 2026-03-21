#pragma once

#include "point3.hh"
#include "ray.hh"
#include "vector3.hh"

class Hit
{
public:
    Point3 point;
    Vector3 normal;
    double t;
    bool front_face;

    Hit()
        : point()
        , normal()
        , t(0.0)
    {}
    Hit(Point3 point, Vector3 normal, double t)
        : point(point)
        , normal(normal)
        , t(t)
    {}

    void set_face_normal(const Ray &ray, const Vector3 &outward_normal)
    {
        normal = ray.direction.dot(outward_normal) < 0 ? outward_normal
                                                       : -outward_normal;
    }
};
