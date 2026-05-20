#include <algorithm>
#include <cmath>
#include <iostream>

#include "box.hh"
#include "camera.hh"
#include "diffuse_light.hh"
#include "henyey_greenstein.hh"
#include "heterogeneous_volume.hh"
#include "image.hh"
#include "metal.hh"
#include "perlin.hh"
#include "point3.hh"
#include "renderer.hh"
#include "scene.hh"
#include "solid_color.hh"
#include "vector3.hh"

inline double smoothstep(double edge0, double edge1, double x)
{
    double t = std::clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

double dist_sq_point_segment(const Vector3 &p, const Vector3 &a,
                             const Vector3 &b)
{
    Vector3 ab = b - a;
    Vector3 ap = p - a;
    double t = ap.dot(ab) / ab.dot(ab);
    t = std::clamp(t, 0.0, 1.0);
    Vector3 closest = a + ab * t;
    return (p - closest).dot(p - closest);
}

class SkyGradientTexture : public Texture
{
public:
    RGB value(const Point3 &p) const override
    {
        Vector3 dir = Vector3(p).normalize();

        double t = 0.5 * (dir.y + 1.0);

        RGB color_bottom(255, 100, 80);
        RGB color_top(10, 20, 60);

        return color_bottom * (1.0 - t) + color_top * t;
    }
};

class IsimColorTexture : public Texture
{
public:
    RGB value(const Point3 &p) const override
    {
        if (p.x < -2.5)
            return RGB(0, 200, 255);
        if (p.x < -0.5)
            return RGB(255, 50, 150);
        if (p.x < 1.5)
            return RGB(255, 220, 0);
        return RGB(50, 255, 100);
    }
};

class IsimCloudTexture : public Texture
{
public:
    IsimCloudTexture(Point3 box_min, Point3 box_max, double cloud_scale)
        : box_min_(box_min)
        , box_max_(box_max)
        , cloud_scale_(cloud_scale)
    {
        define_letters_segments();
    }

    RGB value(const Point3 &p) const override
    {
        if (p.x < box_min_.x || p.x > box_max_.x || p.y < box_min_.y
            || p.y > box_max_.y || p.z < box_min_.z || p.z > box_max_.z)
        {
            return RGB(0, 0, 0);
        }

        const double letter_radius = 0.25;
        const double smooth_width = 0.2;

        double text_density = 0.0;
        Vector3 p_vec(p);

        for (const auto &segment : segments_)
        {
            double d_sq =
                dist_sq_point_segment(p_vec, segment.first, segment.second);
            if (d_sq < (letter_radius + smooth_width)
                    * (letter_radius + smooth_width))
            {
                double d = std::sqrt(d_sq);
                text_density = std::max(
                    text_density,
                    smoothstep(letter_radius + smooth_width, letter_radius, d));
            }
        }

        if (text_density < 0.001)
            return RGB(0, 0, 0);

        double n = noise_.fbm(p * cloud_scale_, 5);
        double cloud_noise = std::clamp((n - 0.3) * 3.0, 0.0, 1.0);

        double final_density = text_density * cloud_noise;

        double center_z = (box_min_.z + box_max_.z) * 0.5;
        double depth_fade = smoothstep(1.5, 0.5, std::abs(p.z - center_z));

        final_density *= depth_fade;
        return RGB(final_density, final_density, final_density);
    }

private:
    Point3 box_min_, box_max_;
    double cloud_scale_;
    Perlin noise_;
    std::vector<std::pair<Vector3, Vector3>> segments_;

    void define_letters_segments()
    {
        segments_.push_back(
            { Vector3(-3.5, 1.5, -2.5), Vector3(-3.5, 3.5, -2.5) });
        segments_.push_back(
            { Vector3(-3.8, 1.5, -2.5), Vector3(-3.2, 1.5, -2.5) });
        segments_.push_back(
            { Vector3(-3.8, 3.5, -2.5), Vector3(-3.2, 3.5, -2.5) });

        segments_.push_back(
            { Vector3(-2.0, 3.5, -2.5), Vector3(-1.0, 3.5, -2.5) });
        segments_.push_back(
            { Vector3(-2.0, 3.5, -2.5), Vector3(-2.0, 2.5, -2.5) });
        segments_.push_back(
            { Vector3(-2.0, 2.5, -2.5), Vector3(-1.0, 2.5, -2.5) });
        segments_.push_back(
            { Vector3(-1.0, 2.5, -2.5), Vector3(-1.0, 1.5, -2.5) });
        segments_.push_back(
            { Vector3(-2.0, 1.5, -2.5), Vector3(-1.0, 1.5, -2.5) });

        segments_.push_back(
            { Vector3(0.5, 1.5, -2.5), Vector3(0.5, 3.5, -2.5) });
        segments_.push_back(
            { Vector3(0.2, 1.5, -2.5), Vector3(0.8, 1.5, -2.5) });
        segments_.push_back(
            { Vector3(0.2, 3.5, -2.5), Vector3(0.8, 3.5, -2.5) });

        segments_.push_back(
            { Vector3(2.0, 1.5, -2.5), Vector3(2.0, 3.5, -2.5) });
        segments_.push_back(
            { Vector3(3.0, 1.5, -2.5), Vector3(3.0, 3.5, -2.5) });
        segments_.push_back(
            { Vector3(2.0, 3.5, -2.5), Vector3(2.5, 2.5, -2.5) });
        segments_.push_back(
            { Vector3(3.0, 3.5, -2.5), Vector3(2.5, 2.5, -2.5) });
    }
};

int main()
{
    Scene scene;

    auto *sky_mat = new DiffuseLight(new SkyGradientTexture(), 1.2);
    scene.addObject(new Box(Point3(-50.0, -10.0, -50.0),
                            Point3(50.0, 50.0, 50.0), sky_mat));

    auto *sol_mat = new Metal(new SolidColor(RGB(15, 15, 20)));
    scene.addObject(
        new Box(Point3(-40.0, -0.5, -40.0), Point3(40.0, 0.0, 40.0), sol_mat));

    auto *soleil_mat =
        new DiffuseLight(new SolidColor(RGB(255, 240, 220)), 30.0);
    scene.addLight(new Box(Point3(-10.0, 20.0, -10.0), Point3(10.0, 20.1, 10.0),
                           soleil_mat));

    Point3 vol_min(-5.0, 1.0, -4.0);
    Point3 vol_max(5.0, 4.0, -1.0);

    auto *isim_shape_tex = new IsimCloudTexture(vol_min, vol_max, 6.0);

    auto *isim_color_tex = new IsimColorTexture();

    auto *cloud_mat = new HenyeyGreenstein(isim_color_tex, 0.5);

    auto *boundary = new Box(vol_min, vol_max, nullptr);

    scene.addObject(
        new HeterogeneousVolume(boundary, 30.0, isim_shape_tex, cloud_mat));

    constexpr int width = 500;
    constexpr int height = 500;
    Image image(width, height);

    Camera camera(Point3(0.0, 3.0, 7.0), Point3(0.0, 2.5, -2.5),
                  Vector3(0.0, 1.0, 0.0), 60.0, 1.0, 1.0);

    Renderer renderer;
    renderer.maxDepth = 20;

    renderer.samplesPerPixel = 100;

    std::cout << "Demarrage du rendu 'ISIM Multicolore'...\n";
    renderer.render(scene, camera, image);
    image.save("isim_colors.ppm");

    std::cout << "Image sauvegardee dans isim_colors.ppm\n";

    return 0;
}
