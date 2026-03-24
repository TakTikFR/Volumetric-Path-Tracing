#include "vector3.hh"

#include <cmath>

#include "point3.hh"
#include "utils.hh"

Vector3::Vector3(const Point3 &p)
    : x(p.x)
    , y(p.y)
    , z(p.z)
{}

Vector3 Vector3::operator+(double s) const
{
    return { x + s, y + s, z + s };
}
Vector3 Vector3::operator-(double s) const
{
    return { x - s, y - s, z - s };
}
Vector3 Vector3::operator*(double s) const
{
    return { x * s, y * s, z * s };
}
Vector3 Vector3::operator/(double s) const
{
    if (s == 0.0f)
        throw std::runtime_error("Vector3: division by zero.");
    return { x / s, y / s, z / s };
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
    return { x + v.x, y + v.y, z + v.z };
}
Vector3 Vector3::operator-(const Vector3 &v) const
{
    return { x - v.x, y - v.y, z - v.z };
}
Vector3 Vector3::operator*(const Vector3 &v) const
{
    return { x * v.x, y * v.y, z * v.z };
}
Vector3 Vector3::operator/(const Vector3 &v) const
{
    if (v.x == 0.0f || v.y == 0.0f || v.z == 0.0f)
        throw std::runtime_error("Vector3: division by zero.");
    return { x / v.x, y / v.y, z / v.z };
}

Vector3 Vector3::operator-() const
{
    return { -x, -y, -z };
}

double Vector3::dot(const Vector3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

double Vector3::norm() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize() const
{
    const double n = norm();
    if (n == 0.0f)
        throw std::runtime_error("Vector3::normalize: null vector.");
    return { x / n, y / n, z / n };
}

Vector3 random_vector()
{
    return Vector3(random_double(), random_double(), random_double());
}

Vector3 random_vector(double min, double max)
{
    return Vector3(random_double(min, max), random_double(min, max),
                   random_double(min, max));
}

Vector3 Vector3::random_unit_vector()
{
    while (true)
    {
        Vector3 r = random_vector(-1.0, 1.0);
        double lensq = r.dot(r);
        if (1e-160 < lensq && lensq <= 1)
            return r / sqrt(lensq);
    }
}

Vector3 Vector3::random_on_hemisphere(const Vector3 &normal)
{
    Vector3 r = random_unit_vector();
    if (r.dot(normal) > 0.0)
        return r;
    else
        return -r;
}

Vector3 Vector3::cross(const Vector3 &v) const
{
    return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
}

std::ostream &operator<<(std::ostream &out, const Vector3 &v)
{
    return out << '<' << v.x << ", " << v.y << ", " << v.z << '>';
}