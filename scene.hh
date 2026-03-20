#pragma once

#include <vector>

#include "camera.hh"
#include "light.hh"
#include "object.hh"

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera camera;

    Scene() = default;
    ~Scene() = default;

    void addObject(Object* object) { objects.push_back(object); }
    void addLight(Light* light) { lights.push_back(light); }
};