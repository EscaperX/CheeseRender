#include "parse_scene.h"
#include "pugixml.hpp"
#include "light.h"
#include "render.h"
#include <regex>
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

inline void from_json(const json &j, Vector3f &v)
{
    if (j.size() == 1)
    {
        v = Vector3f(j.get<float>());
        return;
    }
    assert(j.size() == 3 && "vector should be with the length of 3");
    for (int i = 0; i < j.size(); i++)
        j.at(i).get_to(v[i]);
}

void parse_camera(const json &j, Camera &camera)
{

    if (j.contains("transform"))
    {
        if (j.contains("o"))
        {
            j["o"].get_to(camera.position);
        }
    }
    if (j.contains("fov"))
    {
        j["fov"].get_to(camera.fov);
    }

    if (j.contains("resolution"))
    {
        j["resolution"].at(0).get_to(camera.width);
        j["resolution"].at(1).get_to(camera.height);
    }
}
void parse_sampler(const json &j, Render_Option &opt)
{
    if (j.contains("samples"))
    {
        j["samples"].get_to(opt.spp);
    }
}
Scene parse_scene_json(const std::filesystem::path &path)
{
    // open file
    std::ifstream stream(path, std::ifstream::in);

    json j = json::parse(stream,
                         /* callback */ nullptr,
                         /* allow exceptions */ true,
                         /* ignore_comments */ true);

    Camera camera;
    std::vector<Shape> shapes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Image> textures;
    if (j.contains("camera"))
    {
        parse_camera(j["camera"], camera);
    }
    if (j.contains("sampler"))
    {
        parse_sampler(j["sampler"], Render_Option::instance());
    }
    if (j.contains("background"))
    {

        parse_envmap(j["background"]);
    }
}