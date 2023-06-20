#include "parse_scene.h"

#include <tiny_obj_loader.h>
#include <iostream>

#include <regex>

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
        if (id == -1)
            return Vector2f{0.0f, 0.0f};
        return Vector2f{uvs[2 * id], uvs[2 * id + 1]};
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
            if (id1.normal_index == -1 || id2.normal_index == -1 || id3.normal_index == -1)
            {
                size_t id = tm.positions.size();
                auto n = crossProduct(tm.positions[id - 3] - tm.positions[id - 1], tm.positions[id - 2] - tm.positions[id - 1]).normalized();
                tm.normals.push_back(n);
                tm.normals.push_back(n);
                tm.normals.push_back(n);
            }
            else
            {
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
Scene parse_scene(const std::filesystem::path &path)
{
    if (ends_with(path.string(), "xml"))
    {
        return parse_scene_xml(path);
    }
    else if (ends_with(path.string(), "json"))
    {
        return parse_scene_json(path);
    }
}
