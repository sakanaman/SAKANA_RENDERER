#ifndef PERLIN_H
#define PERLIN_H
#include "vec3.h"

inline double perlin_interp(Vec3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                Vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
            }
    return accum;
}

class perlin
{
  public:
    double noise(const Vec3 &p) const
    {
        double u = p.x - floor(p.x);
        double v = p.y - floor(p.y);
        double w = p.z - floor(p.z);

        int i = floor(p.x);
        int j = floor(p.y);
        int k = floor(p.z);
        Vec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
        return perlin_interp(c, u, v, w);
    }

    double turb(const Vec3 &p, int depth = 7) const
    {
        double accum = 0;
        Vec3 temp_p = p;
        double weight = 1.0;
        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p = 2 * temp_p;
        }
        return fabs(accum);
    }

    static Vec3 *ranvec;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

static Vec3 *perlin_generate()
{
    Vec3 *p = new Vec3[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = normalize(Vec3(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
    }
    return p;
}

void permute(int *p, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = int(rnd() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int *perlin_generate_perm()
{
    int *p = new int[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

Vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();

#endif