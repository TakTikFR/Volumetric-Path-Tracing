#include "heterogeneous_volume.hh"

#include <cmath>

#include "utils.hh"

std::optional<Hit> HeterogeneousVolume::intersect(const Ray &ray,
                                                  const interval &ray_int) const
{
    std::optional<Hit> hit_in = boundary_->intersect(ray, interval::universe);
    if (!hit_in)
        return std::nullopt;

    std::optional<Hit> hit_out =
        boundary_->intersect(ray, interval(hit_in->t + 0.0001, infinity));
    if (!hit_out)
        return std::nullopt;

    if (hit_in->t < ray_int.min)
        hit_in->t = ray_int.min;
    if (hit_out->t > ray_int.max)
        hit_out->t = ray_int.max;
    if (hit_in->t >= hit_out->t)
        return std::nullopt;
    if (hit_in->t < 0)
        hit_in->t = 0;

    double ray_length = ray.direction.norm();
    double t_current = hit_in->t;

    while (true)
    {
        double dt = -std::log(random_double()) / density_majorant_;
        t_current += dt / ray_length;

        if (t_current >= hit_out->t)
        {
            Hit hit(ray.at(hit_out->t), Vector3(1, 0, 0), hit_out->t,
                    material_);
            hit.front_face = true;
            hit.transmittance = 1.0;
            hit.is_transmission = true;
            return hit;
        }

        Point3 p = ray.at(t_current);
        RGB tex_val = density_texture_->value(p);
        double local_density =
            density_majorant_ * (tex_val.r + tex_val.g + tex_val.b) / 3.0;

        if (random_double() < local_density / density_majorant_)
        {
            Hit hit(p, Vector3(1, 0, 0), t_current, material_);
            hit.front_face = true;
            hit.transmittance = 1.0;
            hit.is_transmission = false;
            return hit;
        }
    }
}