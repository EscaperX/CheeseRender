#include "parse_scene.h"
#include "pugixml.hpp"
#include "light.h"
#include <regex>
std::vector<std::string> split_string(const std::string &str, const std::regex &delim_regex)
{
    std::sregex_token_iterator first{begin(str), end(str), delim_regex, -1}, last;
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

MeshLight parse_light(pugi::xml_node node)
{
    std::string name_value = node.attribute("mtlname").value();
    MeshLight l;
    l.name = name_value;
    l.radiance = parse_vector3f(node.attribute("radiance").value());
    return l;
}

Scene parse_scene_xml(const std::filesystem::path &path)
{
    Camera camera;
    std::vector<Shape> shapes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Image> textures;

    std::map<std::string, Light> light_map;
    std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;

    pugi::xml_document file;
    pugi::xml_parse_result result = file.load_file(path.c_str());

    if (!result)
    {
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << std::endl;
        throw std::runtime_error("XML Parsing Error!");
    }
    for (auto child : file.children())
    {
        std::string name = child.name();
        if (name == "camera")
            camera = parse_camera(child);
        if (name == "light")
        {
            MeshLight l = parse_light(child);
            light_map[l.name] = l;
        }
    }

    std::filesystem::path obj_path(path);
    obj_path.replace_extension(".obj");
    parse_obj(obj_path, shapes, materials, textures, lights, light_map);
    return Scene(camera, shapes, materials, lights, textures);
}