#ifndef CHEESE_PARSE_SCENE_H
#define CHEESE_PARSE_SCENE_H

#include "scene.h"
#include <filesystem>
#include <iostream>

Scene parse_scene(const std::filesystem::path &path);

bool parse_obj(const std::filesystem::path &path, std::vector<Shape> &shape, std::vector<Material> &materials, std::vector<Image> &textures, std::vector<Light> &lights, const std::map<std::string, Light> &lightmap);

#endif