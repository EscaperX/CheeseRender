#ifndef CHEESE_LIGHT_H
#define CHEESE_LIGHT_H

#include "cheese.h"

struct Texture;

struct MeshLight
{
    std::string name;
    int shape_id;
    Vector3f radiance;
};

struct EnvironmentMap
{
    Texture envmap;
};

using Light = std::variant<MeshLight, EnvironmentMap>;

Vector3f get_light_radiance(const Light &light);

#endif
