#pragma once

#include <ostream>
#include <stdexcept>

class Vector3;

class Point3
{
public:
    double x, y, z;

    Point3(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x)
        , y(y)
        , z(z)
    {}

    explicit Point3(const Vector3 &v);

    Point3 operator+(double scalar) const;
    Point3 operator-(double scalar) const;
    Point3 operator*(double scalar) const;
    Point3 operator/(double scalar) const;

    Point3 operator+(const Point3 &other) const;
    Vector3 operator-(const Point3 &other) const;
    Point3 operator*(const Point3 &other) const;
    Point3 operator/(const Point3 &other) const;

    Point3 operator+(const Vector3 &v) const;
    Vector3 operator-(const Vector3 &v) const;

    friend std::ostream &operator<<(std::ostream &out, const Point3 &p);
};