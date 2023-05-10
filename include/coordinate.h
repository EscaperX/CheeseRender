#ifndef CHEESE_COORDINATE_H
#define CHEESE_COORDINATE_H

#include "cheese.h"

struct Coordinate
{
    Coordinate() : x(Vector3f{1.0f, 0.0f, 0.0f}), y(Vector3f{0.0f, 1.0f, 0.0f}), z(Vector3f{0.0f, 0.0f, 1.0f}) {}
    Coordinate(Vector3f xx, Vector3f yy, Vector3f zz) : x(xx), y(yy), z(zz) {}
    Coordinate(Vector3f n) : z(n.normalized())
    {
        /*if (n[2] < float(-1.0 + 1e-6))
        {
            x = Vector3f{0, -1, 0};
            y = Vector3f{-1, 0, 0};
        }
        else
        {
            float a = 1 / (1 + n[2]);
            float b = -n[0] * n[1] * a;
            x = Vector3f{1 - n[0] * n[0] * a, b, -n[0]};
            y = Vector3f{b, 1 - n[1] * n[1] * a, -n[1]};
        }*/
        Vector3f a = (fabs(n.x) > 0.9) ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
        y = crossProduct(n, a).normalized();
        x = crossProduct(n, y);
    }
    Vector3f x, y, z;
};

Vector3f to_local(const Coordinate &coord, const Vector3f &v);
Vector3f to_world(const Coordinate &coord, const Vector3f &v);

#endif