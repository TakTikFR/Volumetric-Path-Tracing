#include "renderer.hh"

#include <iostream>

#include "interval.hh"
#include "material.hh"
#include "utils.hh"

RGB Renderer::rayColor(const Ray &ray, const Scene &scene, int depth) const
{
    if (depth <= 0)
        return RGB(0, 0, 0);

    interval ray_int(0.001, infinity);
    Hit closestHit;
    bool hitAnything = false;

    for (Object *obj : scene.objects)
    {
        std::optional<Hit> result = obj->intersect(ray, ray_int);
        if (result)
        {
            ray_int.max = result->t;
            closestHit = *result;
            hitAnything = true;
        }
    }

    if (!hitAnything)
        return background;

    Material *material = closestHit.material;
    RGB emitted = material->emitted(closestHit.point);
    RGB attenuation;
    Ray scattered;

    if (material->scatter(ray, closestHit, attenuation, scattered))
        return emitted + (attenuation * rayColor(scattered, scene, depth - 1));

    return emitted;
}

void Renderer::render(const Scene &scene, const Camera &camera,
                      Image &image) const
{
    const int width = image.width;
    const int height = image.height;

    constexpr double focalLength = 1.0;
    constexpr double viewportHeight = 2.0;
    constexpr double viewportWidth = viewportHeight;

    const Vector3 viewportU = Vector3(viewportWidth, 0.0, 0.0);
    const Vector3 viewportV = Vector3(0.0, -viewportHeight, 0.0);
    const Vector3 pixelDeltaU = viewportU / static_cast<double>(width);
    const Vector3 pixelDeltaV = viewportV / static_cast<double>(height);

    const Vector3 viewportUpperLeft = Vector3(camera.center)
        - Vector3(0.0, 0.0, focalLength) - viewportU / 2.0 - viewportV / 2.0;

    const Vector3 pixel00 =
        viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5;

    std::cout << "Rendering: " << width << "x" << height << " pixels."
              << std::endl;

#pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < height; ++j)
    {
        std::cout << "\rScanlines remaining: " << (height - j) << ' '
                  << std::flush;
        for (int i = 0; i < width; ++i)
        {
            double r = 0, g = 0, b = 0;
            for (int s = 0; s < samplesPerPixel; ++s)
            {
                double u = static_cast<double>(i) + random_double();
                double v = static_cast<double>(j) + random_double();
                const Vector3 pixelPos =
                    pixel00 + pixelDeltaU * u + pixelDeltaV * v;
                const Ray ray(camera.center,
                              pixelPos - Vector3(camera.center));

                RGB c = rayColor(ray, scene, maxDepth);
                r += c.r;
                g += c.g;
                b += c.b;
            }
            image.set(i, j,
                      RGB(r / samplesPerPixel, g / samplesPerPixel,
                          b / samplesPerPixel));
        }
    }

    std::cout << "\nRender complete!\n";
}
