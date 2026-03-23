#pragma once

#include "point3.hh"
#include "vector3.hh"

class Perlin
{
public:
    Perlin();
    double noise(const Point3 &p) const;

    static const int point_count = 256;

    Vector3 *ranvec;

    int *perm_x;
    int *perm_y;
    int *perm_z;

    static int *perlin_generate_perm();
    static void permute(int *p);

    static double trilinear_interpolation(Vector3 c[2][2][2], double u,
                                          double v, double w);
    double turbulance(const Point3& p, int depth) const;
};
