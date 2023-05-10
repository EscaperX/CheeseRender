#ifndef CHEESE_PARSE_SCENE_H
#define CHEESE_PARSE_SCENE_H

#include "scene.h"
#include <filesystem>
#include <iostream>

bool parse_obj(const std::filesystem::path &path, std::vector<Shape> &shape, std::vector<Material> &materials);

Scene parse_scene(const std::filesystem::path &path);

#endif