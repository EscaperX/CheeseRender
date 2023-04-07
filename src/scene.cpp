#include "scene.h"

#include <iostream>

Scene::Scene(const Camera &_camera,
          const std::vector<Shape> &_shape,
          const std::vector<Material> &_materials,
          const std::vector<Light> &_lights,
          const std::vector<Image> &_textures):
          camera(_camera), shapes(_shape), materials(_materials), lights(_lights), textures(_textures){
            for (auto& l : lights) {
                max_radiance = std::max({max_radiance, l.radiance[0], l.radiance[1], l.radiance[2]});
            }
          }

void info(const Scene& scene) {

    std::cerr << "Meshes Number: " << scene.shapes.size() << std::endl;
    std::cerr << "materials Number: " << scene.materials.size() << std::endl;
    std::cerr << "Lights Number: " << scene.lights.size() << std::endl;

}
