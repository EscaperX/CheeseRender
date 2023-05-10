#ifndef CHEESE_GEOMETRY_H
#define CHEESE_GEOMETRY_H

#include "cheese.h"

#include <vector>

struct Intersection;
struct Ray;
struct BVH_Node;

struct Geometry
{
    int material_id = -1;
    int light_id = -1;
    int shape_id;
};

struct TriangleMesh : public Geometry
{
    std::vector<Vector3f> positions;
    std::vector<Vector3i> indices;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> uvs;
    std::vector<int> materials;
    float total_area;
};

struct Sphere : public Geometry
{
    Vector3f origin;
    float radius;
};

// although there're no material information
struct AABB : public Geometry
{
    Vector3f minn, maxx;
    AABB() : minn(0), maxx(0) {}
    AABB(Vector3f a) : minn(a), maxx(a) {}
    AABB(Vector3f a, Vector3f b) : minn(a), maxx(b) {}
    AABB(Vector3f a, Vector3f b, Vector3f c)
    {
        for (int i = 0; i < 3; i++)
        {
            minn[i] = std::min({a[i], b[i], c[i]});
            maxx[i] = std::max({a[i], b[i], c[i]});
        }
    }
};
using Shape = std::variant<TriangleMesh, Sphere, AABB>;

std::optional<Intersection> compute_intersection(const Shape &shape, const Ray &ray, float tmin = EPSILON, float tmax = INF);
std::optional<Intersection> compute_triangle_intersection(const Ray &ray,
                                                          const Vector3f &v0, const Vector3f &v1, const Vector3f &v2, 
                                                          const Vector3f &normal0, const Vector3f& normal1, const Vector3f& normal2,
                                                          const Vector2f &tex0, const Vector2f &tex1, const Vector2f &tex2, int material_id, int shape_id);
int get_primitive_number(const Shape &shape);
std::shared_ptr<BVH_Node> get_BVH_Node(const Shape &shape, int shape_id);

void set_material_id(Shape &shape, int material_id);
void set_light_id(Shape &shape, int area_light_id);
int get_material_id(const Shape &shape);
int get_light_id(const Shape &shape);
int get_shape_id(const Shape& shape);

float get_area(const Shape &shape);
std::tuple<Vector3f, Vector3f> sample_point_on_shape(const Shape &shape, const Vector2f &uv, const float &w);
// float pdf_point_on_shape(const Shape &shape, const Vector3f &normal, const Vector3f &normal, const float &w);

#endif
