#include "coordinate.h"
Vector3f to_local(const Coordinate &coord, const Vector3f &v)
{
    return Vector3f{dotProduct(v, coord.x), dotProduct(v, coord.y), dotProduct(v, coord.z)};
}

Vector3f to_world(const Coordinate &coord, const Vector3f &v)
{
    return coord.x * v.x + coord.y * v.y + coord.z * v.z;
}