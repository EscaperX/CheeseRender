#include "aabb.h"
AABB compute_union_box(const AABB &a, const AABB &b)
{
    AABB c;
    for (int i = 0; i < 3; i++)
    {
        c.minn[i] = std::min(a.minn[i], b.minn[i]);
        c.maxx[i] = std::max(a.maxx[i], b.maxx[i]);
    }
    return c;
}
Vector3f get_centroid(const AABB &box)
{
    return (box.maxx + box.minn) * 0.5f;
}
Vector3f get_diagonal(const AABB &box)
{
    return box.maxx - box.minn;
}
int get_split_dim(const AABB &box)
{
    Vector3f d = get_diagonal(box);
    if (d.x > d.y && d.x > d.z)
        return 0;
    else if (d.y > d.z)
        return 1;
    else
        return 2;
}

struct get_bouding_box_op
{
    AABB operator()(const Sphere &sphere) const;
    AABB operator()(const TriangleMesh &mesh) const;
    AABB operator()(const AABB &box) const;
};

// TODO : Implement for sphere
AABB get_bouding_box_op::operator()(const Sphere &sphere) const
{
    return AABB{};
}

AABB get_bouding_box_op::operator()(const TriangleMesh &mesh) const
{
    Vector3f maxx = mesh.positions[0], minn = mesh.positions[0];
    for (auto &pos : mesh.positions)
    {
        for (int i = 0; i < 3; i++)
            maxx[i] = std::max(maxx[i], pos[i]),
            minn[i] = std::min(minn[i], pos[i]);
    }
    return AABB{-1, minn, maxx};
}

AABB get_bouding_box_op::operator()(const AABB &box) const
{
    return box;
}

AABB get_bounding_box(const Shape &shape)
{
    return std::visit(get_bouding_box_op{}, shape);
}