#include "bvh.h"
#include "intersection.h"
#include "ray.h"

void build_BVH(Scene& scene, const std::vector<Shape> &shapes)
{
    Record::instance().start_function("Build BVH");
    std::vector<std::shared_ptr<BVH_Node>> roots;
    float scale;
    for (int i = 0; i < shapes.size(); i++)
    {
        roots.emplace_back(get_BVH_Node(shapes[i], i));
        for (int j = 0; j < 3; j++) scale = std::max(scale, roots.back()->box.maxx[j] - roots.back()->box.minn[j]);
    }
    scene.BVH_root = roots;
    scene.scale = scale;
    Record::instance().end_function("Build BVH");
    
}
std::optional<Intersection> traversal_BVH(const TriangleMesh &mesh, std::shared_ptr<BVH_Node> node, const Ray &ray, float tmin, float tmax)
{
    std::optional<Intersection> temp = compute_intersection(node->box, ray, tmin, tmax);
    if (!temp.has_value())
        return std::nullopt;
    std::optional<Intersection> inter = std::nullopt;
    if (!node->left || !node->right)
    {
        for (int i = 0; i < node->primitives.size(); i++)
        {
            auto &indices = node->primitives[i];
            auto &x = mesh.positions[indices[0]];
            auto &y = mesh.positions[indices[1]];
            auto &z = mesh.positions[indices[2]];
            auto &normal0 = mesh.normals[indices[0]];
            auto& normal1 = mesh.normals[indices[1]];
            auto& normal2 = mesh.normals[indices[2]];
            auto &tex0 = mesh.uvs[indices[0]];
            auto &tex1 = mesh.uvs[indices[1]];
            auto &tex2 = mesh.uvs[indices[2]];
            auto inter_temp = compute_triangle_intersection(ray, x, y, z, normal0, normal1, normal2, tex0, tex1, tex2, mesh.material_id, mesh.shape_id);
            set_intersection(inter, inter_temp);
        }
        return inter;
    }

    if (node->left)
    {
        set_intersection(inter, traversal_BVH(mesh, node->left, ray, tmin, tmax));
    }
    if (node->right)
    {
        set_intersection(inter, traversal_BVH(mesh, node->right, ray, tmin, inter.has_value() ? inter.value().distance : tmax));
    }
    return inter;
}