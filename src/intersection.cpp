#include "intersection.h"
#include "geometry.h"
std::optional<Intersection> intersect(const Scene &scene, const Ray &ray)
{
    std::optional<Intersection> inter = std::nullopt;
    if (scene.BVH_root.size() > 0)
    {
        for (int i = 0; i < scene.shapes.size(); i++)
        {
            auto &root = scene.BVH_root[i];
            auto it = traversal_BVH(std::get<TriangleMesh>(scene.shapes[i]), root, ray, EPSILON, inter.has_value() ? inter.value().distance : INF);
            set_intersection(inter, it);
        }
    }
    else
    {
        for (int i = 0; i < scene.shapes.size(); i++)
        {
            auto &shape = scene.shapes[i];
            auto it = compute_intersection(shape, ray, EPSILON, inter.has_value() ? inter.value().distance : INF);
            set_intersection(inter, it);
        }
    }
    return inter;
}

bool set_intersection(std::optional<Intersection> &dest, const std::optional<Intersection> &src)
{
    if (!src.has_value())
        return false;
    if (!dest.has_value())
    {
        dest = src;
        return true;
    }
    if (dest.value().distance > src.value().distance)
    {
        dest = src;
        return true;
    }
    return false;
}