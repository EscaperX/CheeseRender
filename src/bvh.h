#ifndef CHEESE_BVH_H
#define CHEESE_BVH_H

#include "cheese.h"
#include "geometry.h"

struct AABB;
struct Intersection;
struct Scene;
enum class Split_Method
{
    Naive,
    SAH
};

struct BVH_Option
{
    Split_Method method;
    int primitive_limit;
};

struct BVH_Node
{
    AABB box;
    std::shared_ptr<BVH_Node> left;
    std::shared_ptr<BVH_Node> right;
    float area;
    std::vector<Vector3i> primitives;
    int shape_id;
    int shape_num;
};
void build_BVH(Scene& scene, const std::vector<Shape>& shapes);
std::optional<Intersection> traversal_BVH(const TriangleMesh &mesh, std::shared_ptr<BVH_Node> node, const Ray &ray, float tmin = EPSILON, float tmax = INF);
#endif