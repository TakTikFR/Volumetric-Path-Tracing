#pragma once

#include "point3.hh"
#include "ray.hh"
#include "vector3.hh"

class Material;

class Hit
{
public:
    Point3 point;
    Vector3 normal;
    double t;
    bool front_face;
    Material *material;

    Hit()
        : point()
        , normal()
        , t(0.0)
        , front_face(false)
        , material(nullptr)
    {}

    Hit(Point3 point, Vector3 normal, double t, Material *mat = nullptr)
        : point(point)
        , normal(normal)
        , t(t)
        , front_face(false)
        , material(mat)
    {}

    void set_face_normal(const Ray &ray, const Vector3 &outward_normal)
    {
        front_face = ray.direction.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
