#ifndef CHEESE_INTERSECTION_H
#define CHEESE_INTERSECTION_H

#include "cheese.h"
#include "scene.h"
#include "ray.h"
struct Scene;
struct Intersection
{
    Vector3f position;
    Vector3f normal;

    Vector2f st;
    Vector2f uv;
    bool outward;
    float distance = -1;
    int shape_id, primitive_id, material_id;
};

std::optional<Intersection> intersect(const Scene &scene, const Ray &ray);
bool set_intersection(std::optional<Intersection> &dest, const std::optional<Intersection> &src);
#endif