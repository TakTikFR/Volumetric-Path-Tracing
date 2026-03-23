#include "point3.hh"

#include "vector3.hh"

Point3::Point3(const Vector3 &v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
{}

Point3 Point3::operator+(double s) const
{
    return { x + s, y + s, z + s };
}
Point3 Point3::operator-(double s) const
{
    return { x - s, y - s, z - s };
}
Point3 Point3::operator*(double s) const
{
    return { x * s, y * s, z * s };
}
Point3 Point3::operator/(double s) const
{
    if (s == 0.0f)
        throw std::runtime_error("Point3: division by zero.");
    return { x / s, y / s, z / s };
}

Point3 Point3::operator+(const Point3 &p) const
{
    return { x + p.x, y + p.y, z + p.z };
}
Vector3 Point3::operator-(const Point3 &p) const
{
    return { x - p.x, y - p.y, z - p.z };
}
Point3 Point3::operator*(const Point3 &p) const
{
    return { x * p.x, y * p.y, z * p.z };
}
Point3 Point3::operator/(const Point3 &p) const
{
    if (p.x == 0.0f || p.y == 0.0f || p.z == 0.0f)
        throw std::runtime_error("Point3: division by zero.");
    return { x / p.x, y / p.y, z / p.z };
}

Point3 Point3::operator+(const Vector3 &v) const
{
    return { x + v.x, y + v.y, z + v.z };
}
Vector3 Point3::operator-(const Vector3 &v) const
{
    return { x - v.x, y - v.y, z - v.z };
}

std::ostream &operator<<(std::ostream &out, const Point3 &p)
{
    return out << '(' << p.x << ", " << p.y << ", " << p.z << ')';
}