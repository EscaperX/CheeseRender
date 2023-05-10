#ifndef _SCENE_H_
#define _SCENE_H_

#include "cheese.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "geometry.h"
#include "bvh.h"

#include <thread>
struct Scene
{
    Scene(const Camera &_camera,
          const std::vector<Shape> &_shape,
          const std::vector<Material> &_materials,
          const std::vector<Light> &_lights,
          const std::vector<Image> &_textures);
    std::vector<Shape> shapes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Image> textures;
    Camera camera;
    std::vector<std::shared_ptr<BVH_Node>> BVH_root;
    float scale = 100;
    float max_radiance;
};

void info(const Scene &scene);
#endif
