#ifndef CHEESE_AABB_H
#define CHEESE_AABB_H

#include "geometry.h"

AABB get_bounding_box(const Shape &shape);
AABB compute_union_box(const AABB &a, const AABB &b);
Vector3f get_centroid(const AABB &box);
Vector3f get_diagonal(const AABB &box);
int get_split_dim(const AABB &box);
#endif