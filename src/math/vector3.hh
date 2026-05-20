#pragma once

#include <ostream>

class Point3;

class Vector3
{
public:
    double x, y, z;

    Vector3(double x = 0.0f, double y = 0.0f, double z = 0.0f)
        : x(x)
        , y(y)
        , z(z)
    {}

    explicit Vector3(const Point3 &p);

    Vector3 operator+(double scalar) const;
    Vector3 operator-(double scalar) const;
    Vector3 operator*(double scalar) const;
    Vector3 operator/(double scalar) const;

    Vector3 operator+(const Vector3 &v) const;
    Vector3 operator-(const Vector3 &v) const;
    Vector3 operator*(const Vector3 &v) const;
    Vector3 operator/(const Vector3 &v) const;

    Vector3 operator-() const;

    double dot(const Vector3 &v) const;
    double norm() const;
    Vector3 normalize() const;
    static Vector3 random_unit_vector();
    static Vector3 random_on_hemisphere(const Vector3 &normal);
    Vector3 reflect(Vector3 v, Vector3 n) const;
    Vector3 cross(const Vector3 &v) const;

    friend std::ostream &operator<<(std::ostream &out, const Vector3 &v);
};

Vector3 random_vector();
Vector3 random_vector(double min, double max);
