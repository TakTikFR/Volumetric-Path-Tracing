#pragma once

#include "camera.hh"
#include "image.hh"
#include "ray.hh"
#include "scene.hh"

class Renderer
{
public:
    int maxDepth = 100;
    int samplesPerPixel = 500;
    RGB background{ 0, 0, 0 };

    RGB rayColor(const Ray &ray, const Scene &scene, int depth) const;
    void render(const Scene &scene, const Camera &camera, Image &image) const;
};
