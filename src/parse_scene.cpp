#include "parse_scene.h"
#include "pugixml.hpp"
#include <tiny_obj_loader.h>
#include <iostream>

bool parse_obj(const std::filesystem::path &path, std::vector<Shape> &shape, std::vector<Material> &materials, std::vector<Image> &textures, std::vector<Light> &lights, const std::map<std::string, Light> &lightmap)
{
    Record::instance().start_function("parse_obj");
    auto temp = std::filesystem::current_path();
    std::filesystem::current_path(path.parent_path());
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path.filename().string()))
    {
        std::cerr << "Load file error!" << std::endl;
        std::cerr << reader.Error() << std::endl;
        return false;
    }
    // extract data of reader
    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &mats = reader.GetMaterials();
    auto &vertices = attrib.vertices;
    auto &normals = attrib.normals;
    auto &uvs = attrib.texcoords;

    auto get_position = [&vertices](int id)
    { return Vector3f{vertices[3 * id], vertices[3 * id + 1], vertices[3 * id + 2]}; };
    auto get_normal = [&normals](int id)
    {
        return Vector3f{normals[3 * id], normals[3 * id + 1], normals[3 * id + 2]}.normalized();
    };
    auto get_uv = [&uvs](int id)
    {
        if (id == -1) return Vector2f{ 0.0f, 0.0f };
        return Vector2f{uvs[2 * id], uvs[2 * id + 1] };
    };

    for (int i = 0; i < mats.size(); i++)
    {
        Phong phong;
        phong.diffuse = ConstantTexture({mats[i].diffuse[0], mats[i].diffuse[1], mats[i].diffuse[2]});
        phong.specular = ConstantTexture({mats[i].specular[0], mats[i].specular[1], mats[i].specular[2]});
        phong.ior = ConstantTexture(mats[i].ior);
        phong.spec_exponent = ConstantTexture(mats[i].shininess);
        phong.transparent = ConstantTexture({mats[i].transmittance[0], mats[i].transmittance[1], mats[i].transmittance[2]});
        phong.diffuse_map = std::nullopt;
        if (!mats[i].diffuse_texname.empty())
        {
            textures.push_back(Image(mats[i].diffuse_texname.c_str()));
            phong.diffuse_map = std::make_optional<Texture>(ImageTexture(textures.size() - 1));
        }
        materials.emplace_back(phong);
    }

    for (int i = 0; i < shapes.size(); i++)
    {
        TriangleMesh tm;
        tm.total_area = 0;
        tm.light_id = -1;
        tm.shape_id = i;
        for (int j = 0; j < shapes[i].mesh.indices.size(); j += 3)
        {
            auto id1 = shapes[i].mesh.indices[j];
            auto id2 = shapes[i].mesh.indices[j + 1];
            auto id3 = shapes[i].mesh.indices[j + 2];
            tm.positions.push_back(get_position(id1.vertex_index));
            tm.positions.push_back(get_position(id2.vertex_index));
            tm.positions.push_back(get_position(id3.vertex_index));
            if (id1.normal_index == -1 || id2.normal_index == -1 || id3.normal_index == -1) {
                int id = tm.positions.size();
                auto n = crossProduct(tm.positions[id - 3] - tm.positions[id - 1], tm.positions[id - 2] - tm.positions[id - 1]).normalized();
                tm.normals.push_back(n);
                tm.normals.push_back(n);
                tm.normals.push_back(n);
            }
            else {
                tm.normals.push_back(get_normal(id1.normal_index));
                tm.normals.push_back(get_normal(id2.normal_index));
                tm.normals.push_back(get_normal(id3.normal_index));
            }
            tm.uvs.push_back(get_uv(id1.texcoord_index));
            tm.uvs.push_back(get_uv(id2.texcoord_index));
            tm.uvs.push_back(get_uv(id3.texcoord_index));
            tm.materials.push_back(shapes[i].mesh.material_ids[j / 3]);
            int id = tm.positions.size();
            tm.indices.push_back(Vector3i{id - 3, id - 2, id - 1});
            // tm.uvs.push_back(Vector2i{})
            tm.total_area += crossProduct(tm.positions[id - 3] - tm.positions[id - 1], tm.positions[id - 2] - tm.positions[id - 1]).norm() * 0.5;
        }
        tm.material_id = tm.materials[0];
        std::string material_name = mats[tm.material_id].name;
        if (lightmap.count(material_name))
        {
            auto l = lightmap.at(material_name);
            lights.push_back(Light{
                material_name,
                i,
                l.radiance});
            tm.light_id = lights.size() - 1;
        }
        shape.emplace_back(tm);
    }

    std::filesystem::current_path(temp);
    Record::instance().end_function("parse_obj");
    return true;
}
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

Light parse_light(pugi::xml_node node)
{
    std::string name_value = node.attribute("mtlname").value();
    Light l;
    l.name = name_value;
    l.radiance = parse_vector3f(node.attribute("radiance").value());
    return l;
}
Scene parse_scene(const std::filesystem::path &path)
{
    Record::instance().start_function("parse_obj");

    std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    pugi::xml_document file;
    pugi::xml_parse_result result = file.load_file(path.c_str());

    if (!result)
    {
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << std::endl;
        throw std::runtime_error("XML Parsing Error!");
    }
    Camera camera;
    std::vector<Shape> shapes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Image> textures;

    std::map<std::string, Light> light_map;
    for (auto child : file.children())
    {
        std::string name = child.name();
        if (name == "camera")
            camera = parse_camera(child);
        if (name == "light")
        {
            Light l = parse_light(child);
            light_map[l.name] = l;
        }
    }

    std::filesystem::path obj_path(path);
    obj_path.replace_extension(".obj");
    parse_obj(obj_path, shapes, materials, textures, lights, light_map);

    return Scene(camera, shapes, materials, lights, textures);
}
