#include "render.h"
#include "coordinate.h"
#include "intersection.h"
#include "pcg.h"
#include "progress.h"
#include <random>

// stolen from GAMES 101
inline void UpdateProgress(float progress)
{
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
};
inline float get_random_1(pcg32_state &rng)
{
    /*static std::random_device rd_;
    static std::default_random_engine random_engine_(rd_());
    static std::uniform_real_distribution<float> uniform_distribution_(0.f, 1.f);
    return uniform_distribution_(random_engine_);*/
    return next_pcg32_real<float>(rng);
}

std::tuple<Vector3f, Vector3f, Vector3f, float> sample_light(const std::vector<Light> &lights, const std::vector<Shape> &shapes, pcg32_state &rng)
{
    // float emit_area_sum = 0;
    // for (int i = 0; i < lights.size(); i++)
    //     emit_area_sum += get_area(shapes[lights[i].shape_id]);
    // //std::cerr << get_area(shapes[lights[0].shape_id]) << std::endl;
    // float p = get_random_1(rng) * emit_area_sum;
    // float prev_area_sum = 0;
    // for (int i = 0; i < lights.size(); i++)
    // {
    //     prev_area_sum += get_area(shapes[lights[i].shape_id]);
    //     if (p <= prev_area_sum)
    //     {
    //         auto [point, normal] = sample_point_on_shape(shapes[lights[i].shape_id], {get_random_1(rng), get_random_1(rng)}, get_random_1(rng));
    //         auto emission = lights[i].radiance;
    //         float pdf = 1.0f / emit_area_sum;
    //         return {emission, point, normal, pdf};
    //     }
    // }
    int id = int(get_random_1(rng) * lights.size());
    if (id == lights.size())
        id--;
    auto [point, normal] = sample_point_on_shape(shapes[lights[id].shape_id], {get_random_1(rng), get_random_1(rng)}, get_random_1(rng));
    auto emission = lights[id].radiance;
    float pdf = 1.0f / get_area(shapes[lights[id].shape_id]) / lights.size();
    return {emission, point, normal, pdf};
}

Vector3f shade(const Scene &scene, Intersection &inter, Vector3f wo, pcg32_state &rng, int depth = 0)
{
    auto &shape = scene.shapes[inter.shape_id];
    int light_id = get_light_id(shape);
    Vector3f radiance_emission(0.0f);
    if (light_id != -1 && inter.outward && depth == 0)
    {
        radiance_emission = scene.lights[light_id].radiance;
    }

    auto opt = Render_Option::instance();
    if (depth > opt.depth)
        return radiance_emission;
    auto &material = scene.materials[inter.material_id];
    Vector3f radiance_direct(0.0f);
    if (opt.sample_light)
    {
        auto [emission, light_point, light_normal, light_pdf] = sample_light(scene.lights, scene.shapes, rng);
        Vector3f obj2light = light_point - inter.position;
        Vector3f obj2light_dir = obj2light.normalized();
        float dist = obj2light.norm();
        Vector3f biased_position = inter.position + inter.normal * scene.scale * (inter.outward ? EPSILON : -EPSILON);

        auto t = intersect(scene, Ray(biased_position, obj2light_dir));
        // if (t.has_value())std::cerr << t.value().distance << std::endl;
        if (t.has_value() && (light_point - t.value().position).norm() < EPSILON * scene.scale * 10)
        {
            Vector3f f_r = eval(material, wo, obj2light_dir, inter, scene.textures);
            float r2 = dist * dist;
            float cosA = std::max(0.0f, dotProduct(inter.normal.normalized(), obj2light_dir));
            float cosB = std::max(0.0f, dotProduct(light_normal.normalized(), -obj2light_dir));

            radiance_direct = emission * f_r * cosA * cosB / r2 / light_pdf;
        }
    }

    if (!opt.sample_light)
    // if (get_random_1(rng) < opt.threshold)
    {
        Vector2f rnd_uv{get_random_1(rng), get_random_1(rng)};

        auto [scatter_dir_op, pdf] = sample_bsdf(material, wo, inter, scene.textures, rnd_uv); //, get_random_1(rng));
        if (scatter_dir_op.has_value())
        {
            auto scatter_dir = scatter_dir_op.value().normalized();
            // float pdf = pdf_sample_bsdf(material, wo, scatter_dir, inter, scene.textures);
            if (pdf > EPSILON)
            {
                auto next_inter = intersect(scene, Ray(inter.position + inter.normal * scene.scale * (inter.outward > 0 ? EPSILON : -EPSILON), scatter_dir));
                if (next_inter.has_value() && next_inter.value().distance > EPSILON * scene.scale)
                {
                    int light_id = get_light_id(scene.shapes[next_inter.value().shape_id]);
                    if (light_id != -1 && dotProduct(next_inter.value().normal, -scatter_dir) > 0)
                    {
                        Vector3f f_r = eval(material, wo, scatter_dir, inter, scene.textures);
                        float cos = 1; // std::max(.0f, dotProduct(inter.normal.normalized(), scatter_dir));
                        radiance_direct = scene.lights[light_id].radiance * f_r * cos / pdf;
                    }
                }
            }
        }
    }
    // return radiance_emission + radiance_direct;
    if (depth > opt.min_depth && get_random_1(rng) > opt.threshold)
        return radiance_emission + radiance_direct;

    Vector3f radiance_indirect(0.0f);
    {
        Vector2f rnd_uv{get_random_1(rng), get_random_1(rng)};

        auto [scatter_dir_op, pdf] = sample_bsdf(material, wo, inter, scene.textures, rnd_uv);
        if (scatter_dir_op.has_value())
        {
            auto scatter_dir = scatter_dir_op.value().normalized();
            // float pdf = pdf_sample_bsdf(material, wo, scatter_dir, inter, scene.textures);
            if (pdf > EPSILON)
            {
                auto next_inter = intersect(scene, Ray(inter.position + inter.normal * scene.scale * (inter.outward ? EPSILON : -EPSILON), scatter_dir));
                if (next_inter.has_value()) //&& next_inter.value().distance > EPSILON * scene.scale )//&& get_light_id(scene.shapes[next_inter.value().shape_id]) == -1)
                {
                    Vector3f f_r = eval(material, wo, scatter_dir, inter, scene.textures);
                    radiance_indirect = shade(scene, next_inter.value(), -scatter_dir, rng, depth + 1) * f_r / pdf;
                    if (depth > opt.min_depth)
                        radiance_indirect = radiance_indirect / opt.threshold;
                }
            }
        }
    }

    return radiance_emission + radiance_direct + radiance_indirect;
}
Vector3f shade_iterative(const Scene &scene, Intersection &inter, Vector3f wo, pcg32_state &rng, int depth = 0)
{
    Vector3f path_throughput = Vector3f(1.0f, 1.0f, 1.0f);
    Vector3f Li(0.0f);
    const auto &opt = Render_Option::instance();
    for (int bounce = 0; bounce < opt.depth; bounce++)
    {
        auto &shape = scene.shapes[inter.shape_id];
        auto &material = scene.materials[inter.material_id];
        int light_id = get_light_id(shape);

        Vector3f radiance_emission(0.0f);
        if (light_id != -1 && inter.outward && bounce == 0)
        {
            Li = Li + scene.lights[light_id].radiance;
        }

        if (bounce > opt.min_depth)
        {
            if (get_random_1(rng) > opt.threshold)
            {
                break;
            }
            path_throughput = path_throughput / opt.threshold;
        }
        if ()
    }
}
Vector3f gamma_correction(Vector3f pixel, float gamma)
{
    float x = pow(std::clamp(pixel.x, 0.0f, 1.0f), gamma);
    float y = pow(std::clamp(pixel.y, 0.0f, 1.0f), gamma);
    float z = pow(std::clamp(pixel.z, 0.0f, 1.0f), gamma);
    return Vector3f{x, y, z};
}
std::vector<Imagef> render(const Scene &scene)
{
    Record::instance().start_function("Rendering Helper Image");
    int height = scene.camera.height, width = scene.camera.width;
    Imagef depth_map(width, height);
    Imagef normal_map(width, height);
    Imagef final_frame(width, height);

    auto opt = Render_Option::instance();
    std::vector<std::thread> thread_pool;

    int block_height = (height / opt.thread_num) + int(height % opt.thread_num == 0);
    ProgressReporter reporter(height * width * opt.spp);
    for (int thread_id = 0; thread_id < opt.thread_num; thread_id++)
    {
        std::thread t([&scene, &opt, &depth_map, &normal_map, &final_frame, &reporter, thread_id, block_height]()
                      {
                pcg32_state rng = init_pcg32(thread_id);
                int height = scene.camera.height;
                int width = scene.camera.width;
                int enter = thread_id * block_height;
                int exit = (thread_id == opt.thread_num - 1) ? height : (thread_id + 1) * block_height;
                for (int j = enter; j < exit; ++j)
                {
                    for (int i = 0; i < width; ++i) {
                        Vector3f pixel(0.0f), depth(0.0f), normal(0.0f);
                        for (int sp = 0; sp < opt.spp; sp++)
                        {
                            auto& camera = scene.camera;
                            if (i == 654 && j == 949) {
                                puts("fucl");
                            }
                            Ray ray = sample_ray(camera, Vector2f{ 1.0f * i, 1.0f * j } + Vector2f{ next_pcg32_real<float>(rng), next_pcg32_real<float>(rng) });
                            auto v = intersect(scene, ray);
                            if (!v.has_value()) continue;
                            Vector3f color = shade(scene, v.value(), -ray.direction, rng);
                            if (std::max({ color[0], color[1], color[2] }) > scene.max_radiance + EPSILON) continue;
                            pixel = pixel + color * (1.0f / opt.spp);
                            if (opt.gbuffer)
                            {
                                depth = depth + Vector3f{ 100.0f / v.value().distance } *(1.0f / opt.spp);
                                normal = normal + (v.value().normal + Vector3f{ 1.0f }) * 0.5 * (1.0f / opt.spp);
                            }
                        }
                        
                        depth_map.at(i, j) = depth;// gamma_correction(depth, 1.0f / 2.2f);
                        normal_map.at(i, j) = normal; //gamma_correction(normal, 1.0f / 2.2f);
                        final_frame.at(i, j) = pixel;//gamma_correction(pixel, 1.0f / 2.2f);
                }
                    reporter.update(opt.spp * width);
            } });

        thread_pool.emplace_back(std::move(t));
    }
    for (auto &t : thread_pool)
        t.join();

    Record::instance().end_function("Rendering Helper Image");
    return std::vector<Imagef>{depth_map, normal_map, final_frame};
}
// Image path_tracer(const Scene &scene)
// {
// }
