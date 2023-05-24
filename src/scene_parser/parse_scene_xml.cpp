#include "parse_scene.h"
#include "pugixml.hpp"
#include <regex>
std::vector<std::string> split_string(const std::string &str, const std::regex &delim_regex)
{
    std::regex_token_iterator first{begin(str), end(str), delim_regex, -1}, last;
    std::vector<std::string> list{first, last};
    return list;
}
Vector3f parse_vector3f(std::string str)
{
    std::vector<std::string> list = split_string(str, std::regex("(,| )+"));
    Vector3f v;
    if (list.size() == 1)
    {
        v[0] = std::stof(list[0]);
        v[1] = std::stof(list[0]);
        v[2] = std::stof(list[0]);
    }
    else if (list.size() == 3)
    {
        v[0] = std::stof(list[0]);
        v[1] = std::stof(list[1]);
        v[2] = std::stof(list[2]);
    }
    else
    {
        throw std::runtime_error("parse_vector3 failed");
    }
    return v;
}
Vector3f parse_vector3f(pugi::xml_node node)
{
    float x, y, z;
    x = std::stof(node.attribute("x").value());
    y = std::stof(node.attribute("y").value());
    z = std::stof(node.attribute("z").value());
    return Vector3f{x, y, z};
}
Camera parse_camera(pugi::xml_node node)
{
    Camera camera;
    std::string type = node.attribute("type").value();
    camera.width = std::stof(node.attribute("width").value());
    camera.height = std::stof(node.attribute("height").value());
    camera.fov = std::stof(node.attribute("fovy").value());

    for (auto child : node.children())
    {
        std::string name = child.name();
        if (name == "lookat")
            camera.lookat = parse_vector3f(child);
        else if (name == "up")
            camera.up = parse_vector3f(child).normalized();
        else if (name == "eye")
            camera.position = parse_vector3f(child);
    }
    camera.lookat = (camera.lookat - camera.position).normalized();
    camera.right = crossProduct(camera.lookat, camera.up).normalized();
    return camera;
}

Light parse_light(pugi::xml_node node)
{
    std::string name_value = node.attribute("mtlname").value();
    Light l;
    l.name = name_value;
    l.radiance = parse_vector3f(node.attribute("radiance").value());
    return l;
}
