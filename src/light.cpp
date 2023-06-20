#include "light.h"

struct light_radiance_op
{
    Vector3f operator()(const MeshLight &light)
    {
        return light.radiance;
    }
};

Vector3f
get_light_radiance(const Light &light)
{
    return std::visit(light_radiance_op{}, light);
}