#pragma once

#include "point3.hh"
#include "vector3.hh"

class Camera {
public:
    Point3  center;
    Point3  target;
    Vector3 up;

    double alpha;
    double beta;
    double z_min;

    Camera()
        : center(0.0, 0.0, 0.0)
        , target(0.0, 0.0, -1.0)
        , up(0.0, 1.0, 0.0)
        , alpha(90.0)
        , beta(90.0)
        , z_min(1.0)
    {}

    Camera(double alpha, double beta, double z_min)
        : center(0.0, 0.0, 0.0)
        , target(0.0, 0.0, -1.0)
        , up(0.0, 1.0, 0.0)
        , alpha(alpha)
        , beta(beta)
        , z_min(z_min)
    {}

    Camera(const Point3& center, const Point3& target, const Vector3& up,
           double alpha, double beta, double z_min)
        : center(center)
        , target(target)
        , up(up)
        , alpha(alpha)
        , beta(beta)
        , z_min(z_min)
    {}
};