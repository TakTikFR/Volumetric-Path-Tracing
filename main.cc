#include <iostream>
#include <memory>
#include <optional>

#include "box.hh"
#include "camera.hh"
#include "dielectric_texture.hh"
#include "image.hh"
#include "metal_texture.hh"
#include "object.hh"
#include "point3.hh"
#include "point_light.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniforme_texture.hh"
#include "utils.hh"
#include "vector3.hh"

constexpr int MAX_DEPTH = 100;
constexpr int SAMPLES = 300;

RGB rayColor(const Ray &ray, const Scene &scene, int depth)
{
    if (depth <= 0)
        return RGB(0, 0, 0);

    constexpr double tMin = 0.001;
    constexpr double tMax = 1e30;
    Hit closestHit;
    Object *hitObj = nullptr;
    double tClose = tMax;

    for (Object *obj : scene.objects)
    {
        std::optional<Hit> result = obj->intersect(ray);
        if (result && result->t >= tMin && result->t < tClose)
        {
            tClose = result->t;
            closestHit = *result;
            hitObj = obj;
        }
    }

    if (!hitObj)
    {
        Vector3 unitDir = ray.direction.normalize();
        double t = 0.5 * (unitDir.y + 1.0);
        return RGB(static_cast<uint8_t>((1.0 - t) * 255.0 + t * 127.5),
                   static_cast<uint8_t>((1.0 - t) * 255.0 + t * 179.0),
                   static_cast<uint8_t>((1.0 - t) * 255.0 + t * 255.0));
    }

    Texture_Material *material = hitObj->getMaterial();
    RGB color;
    Ray reboundRay;
    if (material->scatter(ray, closestHit, color, reboundRay))
    {
        return color * rayColor(reboundRay, scene, depth - 1);
    }
    else
    {
        return RGB(0, 0, 0);
    }
}

int main()
{
    // ── Scene setup
    // ────────────────────────────────────────────────────────────
    /*
    Scene scene;
    MetalTexture* metal_trans = new MetalTexture(RGB(204, 204, 204));
    DielectricTexture* metal_cyan = new DielectricTexture(RGB(255, 255,
    255), 1.50); UniformeTexture* uniforme_purple = new UniformeTexture(RGB(255,
    255, 0)); UniformeTexture* uniforme_big = new UniformeTexture(RGB(255, 255,
    255));

    Sphere *sphere_trans = new Sphere(Point3(-1, 0.0, -2.0), 0.5, metal_trans);
    Sphere *sphere_cyan = new Sphere(Point3(0.5, 0.0, -1.0), 0.5, metal_cyan);
    Sphere *sphere_purple = new Sphere(Point3(0.0, 1.2, -1.0), 0.5,
    uniforme_purple); Sphere *big_sphere = new Sphere(Point3(0.0, -100.5, -1.0),
    100, uniforme_big);

    scene.addObject(sphere_purple);
    scene.addObject(big_sphere);
    scene.addObject(sphere_cyan);
    scene.addObject(sphere_trans);

    DielectricTexture* verre = new DielectricTexture(RGB(255, 255, 255), 1.10);
    UniformeTexture* uniforme_purple = new UniformeTexture(RGB(255, 0, 255));
    UniformeTexture* uniforme_big = new UniformeTexture(RGB(255, 255, 255));

    Sphere *sphere_trans = new Sphere(Point3(0.0, 0.0, -2.0), 0.5, verre);
    Sphere *sphere_cyan = new Sphere(Point3(0.0, 0.0, -3.0), 0.5,
    uniforme_purple); Sphere *big_sphere = new Sphere(Point3(0.0, -100.5, -1.0),
    100, uniforme_big);

    scene.addObject(sphere_trans);
    scene.addObject(sphere_cyan);
    scene.addObject(big_sphere);


    constexpr double aspectRatio = 16.0 / 9.0;
    constexpr int width = 800;
    constexpr int height = static_cast<int>(width / aspectRatio);

    Image image(width, height);

    constexpr double alpha = 90.0;
    constexpr double beta = alpha / aspectRatio;
    constexpr double zMin = 1.0;

    Camera camera(alpha, beta, zMin);

    constexpr double focalLength = 1.0;
    constexpr double viewportHeight = 2.0;
    constexpr double viewportWidth =
        viewportHeight * (static_cast<double>(width) / height);

    const Vector3 viewportU = Vector3(viewportWidth, 0.0, 0.0);
    const Vector3 viewportV = Vector3(0.0, -viewportHeight, 0.0);

    const Vector3 pixelDeltaU = viewportU / static_cast<double>(width);
    const Vector3 pixelDeltaV = viewportV / static_cast<double>(height);

    const Vector3 viewportUpperLeft = Vector3(camera.center) -
                                      Vector3(0.0, 0.0, focalLength) -
                                      viewportU / 2.0 - viewportV / 2.0;

    const Vector3 pixel00 = viewportUpperLeft + (pixelDeltaU + pixelDeltaV) *
    0.5;
    */

    Scene scene;

    // ── Sol
    // ────────────────────────────────────────────────────────────────────
    scene.addObject(new Sphere(Point3(0.0, -1000.0, -1.0), 1000,
                               new UniformeTexture(RGB(128, 128, 128))));

    // ── Grandes sphères centrales
    // ───────────────────────────────────────────────
    scene.addObject(
        new Sphere(Point3(0.0, 1.0, -2.5), 1.0,
                   new DielectricTexture(RGB(255, 255, 255), 1.50)));

    /*scene.addObject(new Sphere(Point3(-2.5, 1.0, -2.5), 1.0,*/
    /*                           new UniformeTexture(RGB(180, 80, 40))));*/
    /*scene.addObject(new Box(Point3(), Point3(), new UniformeTexture(RGB(180,
     * 80, 40))));*/
    scene.addObject(new Box(Point3(-3.5, 0.0, -3.5), Point3(-1.5, 2.0, -1.5),
                            new MetalTexture(RGB(200, 50, 50))));

    scene.addObject(new Sphere(Point3(2.5, 1.0, -2.5), 1.0,
                               new MetalTexture(RGB(220, 220, 220))));

    // ── Sphères moyennes
    // ───────────────────────────────────────────────────────
    scene.addObject(new Sphere(Point3(-1.2, 0.4, -0.7), 0.4,
                               new MetalTexture(RGB(212, 175, 55))));

    scene.addObject(
        new Sphere(Point3(1.2, 0.4, -0.7), 0.4,
                   new DielectricTexture(RGB(255, 255, 255), 1.33)));

    scene.addObject(new Sphere(Point3(0.0, 0.4, -0.5), 0.4,
                               new UniformeTexture(RGB(80, 180, 80))));

    scene.addObject(new Sphere(Point3(-2.2, 0.35, -1.3), 0.35,
                               new UniformeTexture(RGB(60, 60, 200))));

    scene.addObject(new Sphere(Point3(2.2, 0.35, -1.3), 0.35,
                               new MetalTexture(RGB(180, 100, 100))));

    // ── Petites sphères décoratives
    // ────────────────────────────────────────────
    scene.addObject(
        new Sphere(Point3(-0.6, 0.2, 0.1), 0.2,
                   new DielectricTexture(RGB(255, 255, 255), 2.40)));

    scene.addObject(new Sphere(Point3(0.6, 0.2, 0.1), 0.2,
                               new UniformeTexture(RGB(255, 80, 80))));

    scene.addObject(new Sphere(Point3(-1.6, 0.18, -0.1), 0.18,
                               new MetalTexture(RGB(100, 200, 220))));

    scene.addObject(new Sphere(Point3(1.6, 0.18, -0.1), 0.18,
                               new UniformeTexture(RGB(240, 200, 40))));

    scene.addObject(new Sphere(Point3(0.0, 0.15, 0.2), 0.15,
                               new MetalTexture(RGB(200, 200, 200))));

    scene.addObject(
        new Sphere(Point3(-3.2, 0.25, -2.0), 0.25,
                   new DielectricTexture(RGB(255, 255, 255), 1.50)));

    scene.addObject(new Sphere(Point3(3.2, 0.25, -2.0), 0.25,
                               new UniformeTexture(RGB(160, 40, 200))));

    scene.addObject(new Sphere(Point3(1.0, 0.2, -4.0), 0.2,
                               new MetalTexture(RGB(255, 150, 50))));

    scene.addObject(new Sphere(Point3(-1.0, 0.2, -4.0), 0.2,
                               new UniformeTexture(RGB(40, 220, 180))));

    scene.addObject(
        new Sphere(Point3(0.0, 0.2, -4.3), 0.2,
                   new DielectricTexture(RGB(255, 255, 255), 1.80)));

    // ── Image
    // ──────────────────────────────────────────────────────────────────
    constexpr double aspectRatio = 16.0 / 9.0;
    constexpr int width = 400;
    constexpr int height = static_cast<int>(width / aspectRatio);

    Image image(width, height);

    // ── Caméra légèrement surélevée
    // ────────────────────────────────────────────
    Camera camera(Point3(0.0, 1.5, 2.0), Point3(0.0, 0.5, -3.0),
                  Vector3(0.0, 1.0, 0.0), 60.0, 60.0 / aspectRatio, 1.0);

    constexpr double focalLength = 1.0;
    constexpr double viewportHeight = 2.0;
    constexpr double viewportWidth =
        viewportHeight * (static_cast<double>(width) / height);

    const Vector3 viewportU = Vector3(viewportWidth, 0.0, 0.0);
    const Vector3 viewportV = Vector3(0.0, -viewportHeight, 0.0);

    const Vector3 pixelDeltaU = viewportU / static_cast<double>(width);
    const Vector3 pixelDeltaV = viewportV / static_cast<double>(height);

    const Vector3 viewportUpperLeft = Vector3(camera.center)
        - Vector3(0.0, 0.0, focalLength) - viewportU / 2.0 - viewportV / 2.0;

    const Vector3 pixel00 =
        viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5;

    for (int j = 0; j < height; ++j)
    {
        std::cout << "height" << j << "\n";
        for (int i = 0; i < width; ++i)
        {
            double r = 0, g = 0, b = 0;

            for (int s = 0; s < SAMPLES; ++s)
            {
                double u = static_cast<double>(i) + random_double();
                double v = static_cast<double>(j) + random_double();

                const Vector3 pixelPos =
                    pixel00 + pixelDeltaU * u + pixelDeltaV * v;

                const Ray ray(camera.center, pixelPos - Vector3(camera.center));
                RGB c = rayColor(ray, scene, MAX_DEPTH);

                r += c.r;
                g += c.g;
                b += c.b;
            }

            image.set(i, j,
                      RGB(static_cast<uint8_t>(r / SAMPLES),
                          static_cast<uint8_t>(g / SAMPLES),
                          static_cast<uint8_t>(b / SAMPLES)));
        }
    }

    image.save("output.ppm");
    image.save("output.png");
    std::cout << "Render saved to output.ppm\n";

    return 0;
}
