#include "geometry.h"
#include "aabb.h"
#include "intersection.h"
#include "bvh.h"

void set_material_id(Shape &shape, int material_id)
{
    std::visit([&](auto &s)
               { s.material_id = material_id; },
               shape);
}
void set_light_id(Shape &shape, int light_id)
{
    std::visit([&](auto &s)
               { s.light_id = light_id; },
               shape);
}
int get_material_id(const Shape &shape)
{
    return std::visit([&](const auto &s)
                      { return s.material_id; },
                      shape);
}
int get_light_id(const Shape &shape)
{
    return std::visit([&](const auto &s)
                      { return s.light_id; },
                      shape);
}
int get_shape_id(const Shape& shape)
{
    return std::visit([&](const auto& s)
        { return s.shape_id; },
        shape);
}
struct get_area_op
{
    float operator()(const Sphere &sphere) const
    {
        return 4 * M_PI * sphere.radius * sphere.radius;
    }
    float operator()(const TriangleMesh &mesh) const
    {
        return mesh.total_area;
    }
    float operator()(const AABB &box) const
    {
        return 0;
    }
};

float get_area(const Shape &shape)
{
    return std::visit(get_area_op{}, shape);
}

struct sample_point_on_shape_op
{
    std::tuple<Vector3f, Vector3f> operator()(const Sphere &sphere) const
    {
        float theta = 2.0 * M_PI * uv.x, phi = M_PI * uv.y;
        Vector3f dir(std::cos(phi), std::sin(phi) * std::cos(theta), std::sin(phi) * std::sin(theta));
        auto position = sphere.origin + sphere.radius * dir;
        auto normal = dir;
        return {position, normal};
    }
    std::tuple<Vector3f, Vector3f> operator()(const TriangleMesh &mesh) const
    {
        int id = int(mesh.indices.size() * w);
        if (id == mesh.indices.size()) id--;
        auto &ids = mesh.indices[id];
        auto &v0 = mesh.positions[ids[0]];
        auto &v1 = mesh.positions[ids[1]];
        auto &v2 = mesh.positions[ids[2]];
        float x = std::sqrt(uv.x), y = uv.y;
        auto position = v0 + (v1 - v0) * (1 - x) + (v2 - v0) * x * y;
        auto normal = mesh.normals[ids[0]];
        return {position, normal};
    }
    std::tuple<Vector3f, Vector3f> operator()(const AABB &box) const
    {
        auto position = Vector3f(0.0f);
        auto normal = Vector3f(0.0f);
        return {position, normal};
    }

    // const Vector3 &ref_point;
    // const Vector2 &uv; // for selecting a point on a 2D surface
    const Vector2f &uv;
    const float &w; // for selecting triangles
};
std::tuple<Vector3f, Vector3f> sample_point_on_shape(const Shape &shape, const Vector2f &uv, const float &w)
{
    return std::visit(sample_point_on_shape_op{uv, w}, shape);
}
struct compute_intersection_op
{
    std::optional<Intersection> operator()(const Sphere &sphere) const;
    std::optional<Intersection> operator()(const TriangleMesh &mesh) const;
    std::optional<Intersection> operator()(const AABB &box) const;

    const Ray &ray;
    float tmin, tmax;
};

std::optional<Intersection> compute_intersection_op::operator()(const Sphere &sphere) const
{
    return std::nullopt;
}

std::optional<Intersection> compute_triangle_intersection(const Ray &ray,
                                                          const Vector3f &v0, const Vector3f &v1, const Vector3f &v2,
                                                          const Vector3f &normal0, const Vector3f& normal1, const Vector3f& normal2,
                                                          const Vector2f &tex0, const Vector2f &tex1, const Vector2f &tex2, int material_id, int shape_id)
{
    //// ray intersection
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;

    auto q = crossProduct(ray.direction, edge2);
    auto a = dotProduct(edge1, q);

    if (fabs(a) < EPSILON) return std::nullopt;

    auto f = 1.0f / a;
    auto s = ray.origin - v0;
    auto u = f * dotProduct(s, q);

    if (u < 0 || u > 1)
        return std::nullopt;

    auto k = crossProduct(s, edge1);
    auto v = f * dotProduct(ray.direction, k);

    if (v < 0 || u + v > 1)
        return std::nullopt;

    auto t = f * dotProduct(edge2, k);
    /*if (t < t_min || t_max < t)
        return std::nullopt;*/
    if (t < EPSILON) return std::nullopt;

        Intersection inter;

        //Vector3f outward_normal = (crossProduct(edge1, edge2)).normalized();
        //if (dotProduct(ray.direction, outward_normal) < 0) outward_normal = -outward_normal;

        inter.distance = t;
        inter.normal = crossProduct(edge1, edge2).normalized();//interp_normal(u, v, 1 - u - v, normal0, normal1, normal2).normalized();
        inter.outward = true;
        if (dotProduct(inter.normal, ray.direction) > 0) {
            inter.outward = false;
            inter.normal = -inter.normal;
        }
        inter.position = ray.origin + t * ray.direction;
        inter.st = Vector2f{ u, v };
        inter.material_id = material_id;
        inter.shape_id = shape_id;
        inter.uv = interp_texcoord(u, v, 1 - u - v, tex0, tex1, tex2);
        return std::make_optional<Intersection>(inter);
}
std::optional<Intersection> compute_intersection_op::operator()(const TriangleMesh &mesh) const
{
    std::optional<Intersection> inter = std::nullopt;
    for (auto &indices : mesh.indices)
    {
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
        if (!inter_temp.has_value())
            continue;
        if (inter_temp.value().distance < tmin || inter_temp.value().distance > tmax)
            continue;
        set_intersection(inter, inter_temp);
    }
    return inter;
}

std::optional<Intersection> compute_intersection_op::operator()(const AABB &box) const
{
    float t_enter = tmin, t_exit = tmax;
    for (int i = 0; i < 3; i++) {
        float t_min = (box.minn[i] - ray.origin[i]) * ray.inv_dir[i],
            t_max = (box.maxx[i] - ray.origin[i]) * ray.inv_dir[i];
        if (ray.direction[i] < 0) {
            std::swap(t_min, t_max);
        }
        t_enter = std::max(t_enter, t_min), t_exit = std::min(t_exit, t_max);
    }

    if (t_enter <= t_exit && t_enter < tmax && t_exit >= EPSILON) return std::make_optional<Intersection>(Intersection());
    else return std::nullopt;

}

std::optional<Intersection> compute_intersection(const Shape &shape, const Ray &ray, float tmin, float tmax)
{
    return std::visit(compute_intersection_op{ray, tmin, tmax}, shape);
}

struct get_primitive_op
{
    int operator()(const AABB &shape) const;
    int operator()(const Sphere &sphere) const;
    int operator()(const TriangleMesh &mesh) const;
};

std::shared_ptr<BVH_Node>
build_BVH(const TriangleMesh &mesh, const std::vector<Vector3i> &indices, int shape_id)
{
    std::shared_ptr<BVH_Node> node(new BVH_Node);
    node->shape_id = shape_id;
    node->shape_num = indices.size();

    int dim = 0;
    auto bound_comparator = [&mesh, &dim](const Vector3i &a, const Vector3i &b)
    {
        AABB boxa(mesh.positions[a[0]], mesh.positions[a[1]], mesh.positions[a[2]]);
        AABB boxb(mesh.positions[b[0]], mesh.positions[b[1]], mesh.positions[b[2]]);
        return get_centroid(boxa)[dim] < get_centroid(boxb)[dim];
    };
    if (indices.size() == 1)
    {
        bool first = true;
        node->area = 0;
        for (auto& idx : indices) {
            if (first) node->box = AABB(mesh.positions[idx[0]], mesh.positions[idx[1]], mesh.positions[idx[2]]);
            else node->box = compute_union_box(node->box, AABB(mesh.positions[idx[0]], mesh.positions[idx[1]], mesh.positions[idx[2]]));
            first = false;
            node->primitives.push_back(idx);
            node->area += crossProduct(mesh.positions[idx[1]] - mesh.positions[idx[0]], mesh.positions[idx[2]] - mesh.positions[idx[0]]).norm() * 0.5;
        }

        node->left = nullptr;
        node->right = nullptr;

        return node;
    }
    else
    {
        Vector3f minn, maxx;
        for (auto i = 0; i < indices.size(); i++)
        {
            auto &id = indices[i];
            AABB b(mesh.positions[id[0]], mesh.positions[id[1]], mesh.positions[id[2]]);
            Vector3f c = get_centroid(b);
            if (i == 0)
                minn = c, maxx = c;
            for (int idx = 0; idx < 3; idx++)
            {
                minn[idx] = std::min(minn[idx], c[idx]);
                maxx[idx] = std::max(maxx[idx], c[idx]);
            }
        }
        AABB centroid_box(minn, maxx);
        dim = get_split_dim(centroid_box);
        auto indices_copy = indices;
        std::sort(indices_copy.begin(), indices_copy.end(), bound_comparator);

        auto mid = indices_copy.size() / 2;

        auto leftshapes = std::vector<Vector3i>(indices_copy.begin(), indices_copy.begin() + mid);
        auto rightshapes = std::vector<Vector3i>(indices_copy.begin() + mid, indices_copy.end());

        node->left = build_BVH(mesh, leftshapes, shape_id);
        node->right = build_BVH(mesh, rightshapes, shape_id);

        node->box = compute_union_box(node->left->box, node->right->box);
        node->area = node->left->area + node->right->area;
        return node;
    }
}

struct get_BVH_Node_op
{
    std::shared_ptr<BVH_Node> operator()(const AABB &shape) const
    {
        return nullptr;
    }
    std::shared_ptr<BVH_Node>
    operator()(const Sphere &sphere) const
    {
        return nullptr;
    }
    std::shared_ptr<BVH_Node>
    operator()(const TriangleMesh &mesh) const
    {
        auto indices = mesh.indices;
        return build_BVH(mesh, indices, shape_id);
    }
    int shape_id;
};

std::shared_ptr<BVH_Node> get_BVH_Node(const Shape &shape, int shape_id)
{
    return std::visit(get_BVH_Node_op{shape_id}, shape);
}
