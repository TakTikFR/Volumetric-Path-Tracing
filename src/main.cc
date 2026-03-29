#include <iostream>

#include "box.hh"
#include "camera.hh"
#include "diffuse_light.hh"
#include "henyey_greenstein.hh"
#include "homogeneous_volume.hh"
#include "image.hh"
#include "isotropic.hh"
#include "lambertian.hh"
#include "noise_texture.hh"
#include "point3.hh"
#include "renderer.hh"
#include "scene.hh"
#include "solid_color.hh"
#include "sphere.hh"
#include "vector3.hh"
#include "heterogeneous_volume.hh"

int main()
{
    Scene scene;

    // ─── Textures ──────────────────────────────────────────────────────────
    auto *white_tex = new SolidColor(RGB(180, 180, 180));
    auto *red_tex = new SolidColor(RGB(165, 25, 25));
    auto *green_tex = new SolidColor(RGB(25, 115, 25));
    auto *light_tex = new SolidColor(RGB(255, 255, 255));

    // ─── Materials ─────────────────────────────────────────────────────────
    auto *white = new Lambertian(white_tex);
    auto *red = new Lambertian(red_tex);
    auto *green = new Lambertian(green_tex);
    auto *light_mat = new DiffuseLight(light_tex, 3.0);

    // ─── Room walls ────────────────────────────────────────────────────────
    // Floor
    scene.addObject(
        new Box(Point3(-5.0, 0.0, -5.0), Point3(5.0, 0.1, 5.0), white));
    // Ceiling
    scene.addObject(
        new Box(Point3(-5.0, 4.9, -5.0), Point3(5.0, 5.0, 5.0), white));
    // Back wall
    scene.addObject(
        new Box(Point3(-5.0, 0.0, -5.1), Point3(5.0, 5.0, -5.0), white));
    // Left wall (red)
    scene.addObject(
        new Box(Point3(-5.1, 0.0, -5.0), Point3(-5.0, 5.0, 5.0), red));
    // Right wall (green)
    scene.addObject(
        new Box(Point3(5.0, 0.0, -5.0), Point3(5.1, 5.0, 5.0), green));

    // ─── Light (box on ceiling) ────────────────────────────────────────────
    scene.addLight(
        new Box(Point3(-2.0, 4.89, -4.5), Point3(2.0, 5.0, -0.5), light_mat));

    // ─── Homogeneous volume ────────────────────────────────────────────────
    //auto *volume_tex = new NoiseTexture(RGB(255, 255, 255), 1.0);
    //auto *volume_mat = new HenyeyGreenstein(volume_tex, 0.0);
    //auto *boundary = new Box(Point3(-3.0, 1.0, -3.0), Point3(3.0, 4.8, -1.0), nullptr);
    //scene.addObject(new HomogeneousVolume(boundary, 1.0, volume_mat));

    auto *density_tex = new NoiseTexture(RGB(1.0, 1.0, 1.0), 4.0);
    auto *volume_mat  = new HenyeyGreenstein(new SolidColor(RGB(1, 1, 1)), 0.0);
    auto *boundary = new Box(Point3(-3.0, 1.0, -3.0), Point3(3.0, 4.8, -1.0), nullptr);

    scene.addObject(new HeterogeneousVolume(boundary, 2.0, density_tex, volume_mat));

    // ─── Camera & Image ────────────────────────────────────────────────────
    constexpr int width = 500;
    constexpr int height = 500;
    Image image(width, height);

    Camera camera(Point3(0.0, 2.5, 3.5), Point3(0.0, 2.0, -2.5),
                  Vector3(0.0, 1.0, 0.0), 60.0, 1.0, 1.0);

    // ─── Render ────────────────────────────────────────────────────────────
    Renderer renderer;
    renderer.maxDepth = 100;
    renderer.samplesPerPixel = 1000;

    renderer.render(scene, camera, image);
    image.save("output.ppm");

    std::cout << "Image saved to output.ppm\n";

    return 0;
}
