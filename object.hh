#pragma once

#include <optional>

#include "hit.hh"
#include "image.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Object
{
public:
    explicit Object(Texture_Material *textureMaterial_ = nullptr)
        : textureMaterial_(textureMaterial_)
    {}

    virtual ~Object() = default;

    virtual std::optional<Hit> intersect(const Ray &ray) const = 0;
    virtual Vector3 normal(const Point3 &point) const = 0;
    virtual Texture_Material *getMaterial() const = 0;

protected:
    Texture_Material *textureMaterial_;
};