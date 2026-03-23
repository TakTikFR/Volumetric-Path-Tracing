#pragma once

#include <cmath>
#include <random>

#include "vector3.hh"

inline double random_double()
{
    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline double linear_to_gamma(double linear_component)
{
    return (linear_component > 0.0) ? sqrt(linear_component) : 0.0;
}

inline Vector3 reflect(Vector3 v, Vector3 n)
{
    return v - n * v.dot(n) * 2;
}

inline Vector3 refract(Vector3 v, Vector3 n, double eta)
{
    double cos_theta = fmin((-v).dot(n), 1.0);
    Vector3 ray_out_perpendicular = (v + n * cos_theta) * eta;
    Vector3 ray_out_parallel = n
        * (-sqrt(
            fabs(-ray_out_perpendicular.dot(ray_out_perpendicular) + 1.0)));
    return ray_out_perpendicular + ray_out_parallel;
}
