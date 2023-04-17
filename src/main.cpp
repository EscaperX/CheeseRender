#include "parse_scene.h"
#include "render.h"

#include <map>

int main()
{
    std::map<std::string, std::string> path;
    path.insert({"cornell-simple", "../../assets/cornell-simple/cornell-simple.xml"});
    path.insert({"cornell-dragon", "../../assets/cornell-box/cornell-box.xml"});
    path.insert({"cornell-chair", "../../assets/cornell-box-s/cornell-box.xml"});
    path.insert({"stairscase", "../../assets/stairscase/stairscase.xml"});
    path.insert({"veach-mis", "../../assets/veach-mis/veach-mis.xml"});
    path.insert({"veach-mis-2", "../../assets/veach-mis-2/veach-mis.xml"});
    std::string scene_name = "cornell-dragon";
    // auto scene = parse_scene("../../assets/cornell-simple/cornell-simple.xml");
    auto scene = parse_scene(path[scene_name]);
    // auto scene = parse_scene("../../assets/cornell-box-s/cornell-box.xml");

    info(scene);
    build_BVH(scene, scene.shapes);
    auto res = render(scene);

    auto &img = res.back();
    std::string sample_method;
    if (Render_Option::instance().sample_light)
        sample_method = "_light-only_";
    else
        sample_method = "_bsdf-only_";
    std::string name = scene_name + sample_method + "_spp" + std::to_string(Render_Option::instance().spp) + ".exr";
    imwrite_exr(img, name);
}
