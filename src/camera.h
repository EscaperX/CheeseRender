#ifndef CHEESE_CAMERA_H
#define CHEESE_CAMERA_H

#include "cheese.h"
#include "ray.h"
#include "coordinate.h"

struct Camera
{
    Camera() : position(0.0f), lookat(Vector3f{0, 0, -1}), up(Vector3f{0, 1, 0}), right(Vector3f{1, 0, 0}) {}
    Vector3f position, lookat, up, right;
    float fov;
    int height, width;
};

Ray sample_ray(const Camera &camera, const Vector2f &screen_pos);

#endif
