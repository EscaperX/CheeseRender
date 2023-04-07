#ifndef CHEESE_RAY_H
#define CHEESE_RAY_H
#include "cheese.h"

struct Ray
{
    Ray() {}
    Ray(const Vector3f& o, const Vector3f& d) : origin(o), direction(d.normalized()) { inv_dir = Vector3f{1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z}; }
    Vector3f origin, direction, inv_dir;
};

#endif
