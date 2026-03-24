#pragma once

#include <vector>

#include "camera.hh"
#include "object.hh"

class Scene
{
public:
    std::vector<Object *> objects;
    std::vector<Object *> lights;
    Camera camera;

    Scene() = default;
    ~Scene() = default;

    void addObject(Object *object)
    {
        objects.push_back(object);
    }
    void addLight(Object *light)
    {
        objects.push_back(light);
        lights.push_back(light);
    }
};
