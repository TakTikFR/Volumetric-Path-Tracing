#include "perlin.hh"

#include <cmath>

#include "utils.hh"

Perlin::Perlin()
{
    ranvec = new Vector3[point_count];
    for (int i = 0; i < point_count; ++i)
        ranvec[i] = random_vector(-1.0, 1.0).normalize();

    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}

int *Perlin::perlin_generate_perm()
{
    int *p = new int[point_count];
    for (int i = 0; i < point_count; i++)
        p[i] = i;
    permute(p);
    return p;
}

void Perlin::permute(int *p)
{
    // on mélange la liste
    for (int i = point_count - 1; i > 0; i--)
    {
        int target = static_cast<int>(random_double(0, i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

double Perlin::noise(const Point3 &point) const
{
    // normalisation des coordonnées du point (coordonnées entre 0 et 1)
    double u = point.x - std::floor(point.x);
    double v = point.y - std::floor(point.y);
    double w = point.z - std::floor(point.z);

    // bas gauche du sous-cube du rubiks cube
    int i = static_cast<int>(std::floor(point.x));
    int j = static_cast<int>(std::floor(point.y));
    int k = static_cast<int>(std::floor(point.z));

    // on get les 8 valeurs random des 8 coins
    Vector3 c[2][2][2];
    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                int index_x = perm_x[(i + di) & 255];
                int index_y = perm_y[(j + dj) & 255];
                int index_z = perm_z[(k + dk) & 255];

                c[di][dj][dk] = ranvec[index_x ^ index_y ^ index_z];
            }
        }
    }

    return trilinear_interpolation(c, u, v, w);
}

double Perlin::trilinear_interpolation(Vector3 c[2][2][2], double u, double v,
                                       double w)
{
    // Smoothstep: (3t^2 - 2t^3)
    // casser l'aspect rond et organique
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);

    double accum = 0.0;

    // moyenne pondérée des 8 coins
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                Vector3 weight_v(u - i, v - j, w - k);

                double dot_product = c[i][j][k].dot(weight_v);

                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww)) * dot_product;
            }
        }
    }
    return accum;
}

double Perlin::turbulance(const Point3 &p, int depth) const
{
    double accum = 0.0;
    Point3 temp_p = p;
    double weight = 1.0;

    for (int i = 0; i < depth; i++)
    {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p = temp_p * 2;
    }

    return std::fabs(accum);
}

double Perlin::fbm(const Point3 &p, int depth) const
{
    double value = 0.0;
    double amplitude = 1.0;
    Point3 temp_p = p;

    for (int i = 0; i < depth; i++)
    {
        value += amplitude * std::abs(noise(temp_p));
        amplitude *= 0.5;
        temp_p = Point3(temp_p.x * 2.0, temp_p.y * 2.0, temp_p.z * 2.0);
    }

    return value;
}
