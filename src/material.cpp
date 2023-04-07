#include "material.h"
#include "intersection.h"
#include "vector.h"
inline float Luminance(Vector3f color) {
    return 0.212671f * color.x + 0.715160f * color.y + 0.072169f * color.z;
}
inline Vector3f spherical_dir(float sin_theta, float cos_theta, float phi) {
    return Vector3f{ sin_theta * std::cos(phi), sin_theta * std::sin(phi), cos_theta };
}
Vector3f reflect(const Vector3f& wi, const Vector3f& normal) {
    return 2 * dotProduct(normal, wi) * normal - wi;
}
std::optional<Vector3f> refract(const Vector3f& wi, const Vector3f& normal, float ior) {
    float cos_ni = dotProduct(normal, wi);
    Vector3f normal_real;
    float eta;
    if (cos_ni > 0) {
        eta = 1.f / ior;
        normal_real = normal;
    }
    else {
        cos_ni = -cos_ni;
        eta = ior;
        normal_real = -normal;
    }

    float sin_2_theta = (eta * eta * (1.0f - cos_ni * cos_ni));
    float cos_2_theta = 1.0f - sin_2_theta;
    if (cos_2_theta >= 0) {
        float cos_theta = std::sqrt(cos_2_theta);
        float nK = (eta * cos_ni) - cos_theta;
        Vector3f res = -wi * eta + normal_real * nK;
        return std::make_optional<Vector3f>(res);
    }
    return std::nullopt;
}

inline Vector3f sample_cos_hemisphere(const Vector2f &rnd_param)
{
    float phi = 2 * M_PI * rnd_param.x;
    float tmp = sqrt(std::clamp(1 - rnd_param.y, 0.0f, 1.0f));
    return Vector3f{
        cos(phi) * tmp, sin(phi) * tmp,
        sqrt(std::clamp(rnd_param.y, 0.0f, 1.0f))};
    /*float u = rnd_param.x, v = rnd_param.y;
    float theta = 2 * M_PI * u;
        float phi = acos(2 * v - 1.0);
        float x = sin(phi) * cos(theta);
        float y = cos(phi);
        float z = sin(phi) * sin(theta);
        return Vector3f{ x, y,z };*/
}

struct luminance_op {
    float operator()(const Lambertian& mats) const {
        Vector3f diffuse = eval_texture(mats.reflectance, inter.uv, textures);

        if (mats.diffuse_map.has_value())
        {
            Vector3f diffuse1 = eval_texture(mats.diffuse_map.value(), inter.uv, textures);
            if (diffuse.norm() > 0) diffuse = diffuse * diffuse1;
            else diffuse = diffuse1;
        }
        return Luminance(diffuse);
    }

    float operator()(const Specular& mats) const {
        Vector3f specular = eval_texture(mats.specular, inter.uv, textures);

        return Luminance(specular);
    }
    float operator()(const Transparent& mats)const {
        Vector3f tr = eval_texture(mats.transparent, inter.uv, textures);
        return Luminance(tr);
    }
    float operator()(const Phong& mats) const {
        return luminance_op::operator()(Lambertian{ mats.diffuse, mats.diffuse_map }) +
            luminance_op::operator()(Specular{ mats.specular, mats.spec_exponent }) +
            luminance_op::operator()(Transparent{ mats.transparent, mats.ior });
    }

    const Intersection& inter;
    const std::vector<Image>& textures;
};

float luminance(const Material& material, const Intersection& inter, const std::vector<Image>& texture) {
    return std::visit(luminance_op{ inter, texture }, material);
}

struct eval_op
{
    Vector3f operator()(const Lambertian &mats) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return Vector3f(0.0f);
        }
        Vector3f diffuse = eval_texture(mats.reflectance, vertex.uv, texture_pool);

        if (mats.diffuse_map.has_value())
        {
            Vector3f diffuse1 = eval_texture(mats.diffuse_map.value(), vertex.uv, texture_pool);
            if (diffuse.norm() > 0) diffuse = diffuse * diffuse1;
            else diffuse = diffuse1;
        }
        return diffuse / M_PI * dotProduct(vertex.normal, dir_out);
    }
    Vector3f operator()(const Specular& mats) const {
        Vector3f r = reflect(dir_out, vertex.normal).normalized();
        float alpha = dotProduct(dir_in, r);
        if (alpha > 0) {
            Vector3f ks = eval_texture(mats.specular, vertex.uv, texture_pool);
            Vector3f ns = eval_texture(mats.spec_exponent, vertex.uv, texture_pool);
            Vector3f color = ks * (ns[0] + 2) * (0.5f / M_PI) * std::pow(alpha, ns[0]) * dotProduct(vertex.normal, dir_out);;
            if (color[0] > 500 || color[1] > 500 || color[2] > 500) {
                std::cout << ks << " " << ns << " " << std::endl;
            }
            return color;
        }
        return Vector3f(0.0);
    }
    Vector3f operator()(const Transparent& mats) const {
        return Vector3f(0);
    }
    Vector3f operator()(const Phong &bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return Vector3f(0.0f);
        }
        // Vector3f ni = eval_texture(bsdf.ior, vertex.uv, texture_pool);
        // if (ni == 1)


       
        Lambertian diffuse_comp = Lambertian{bsdf.diffuse, bsdf.diffuse_map};
        Specular spec_comp = Specular{bsdf.specular, bsdf.spec_exponent};
        Vector3f diffuse_color = eval(diffuse_comp, dir_in, dir_out, vertex, texture_pool);
        Vector3f spec_color = eval(spec_comp, dir_in, dir_out, vertex, texture_pool);
        //std::cerr << "diffuse: " << diffuse_color << " spec: " << spec_color << std::endl;
        return diffuse_color + spec_color;// *dotProduct(vertex.normal, dir_out);

    }
    const Vector3f &dir_in;
    const Vector3f &dir_out;
    const Intersection &vertex;
    const std::vector<Image> &texture_pool;
};

Vector3f
eval(const Material &material,
     const Vector3f &dir_in,
     const Vector3f &dir_out,
     const Intersection &inter,
     const std::vector<Image> &texture_pool)
{
    return std::visit(eval_op{dir_in, dir_out, inter, texture_pool}, material);
}
struct sample_bsdf_op
{
    std::tuple<std::optional<Vector3f>, float>  operator()(const Lambertian &bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0)
        {
            return { std::nullopt, 0 };
        }
        Coordinate coord(vertex.normal);
        Vector3f v = (to_world(coord, sample_cos_hemisphere(uv)).normalized());
        return { v, dotProduct(v, vertex.normal) / M_PI };
    }
    std::tuple<std::optional<Vector3f>, float>  operator()(const Phong &bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0)
        {
            return { std::nullopt, 0 };
        }
        Coordinate coord(vertex.normal);
        Vector3f v = (to_world(coord, sample_cos_hemisphere(uv)).normalized());
        return {v, dotProduct(v, vertex.normal) / M_PI};
    }
    std::tuple<std::optional<Vector3f>, float>  operator()(const Specular& bsdf) const
    {
        //Sample a direction on the hemisphere for the half-vector
        float exponent = eval_texture(bsdf.spec_exponent, vertex.uv, texture_pool)[0];
        float cos_theta = std::pow(uv.x, 1 / (exponent + 1));
        float sin_theta = std::sqrt(std::max(0.f, 1 - cos_theta * cos_theta));
        float phi = uv.y * 2 * M_PI;
        Vector3f half = spherical_dir(sin_theta, cos_theta, phi);
        if (dir_in[2] *  half[2] < 0) {
            half = -half;
        }
        //The sampled incident direction is the outgoing direction reflected about the half-vector
        Vector3f dir_out = reflect(dir_in, half);
        float pdf = ((exponent + 1) * std::pow(cos_theta, exponent)) / (2 * M_PI * 4 * dotProduct(dir_out, half));
        if (dotProduct(dir_out, half) <= 0) {
            pdf = 0;
        }
        return { dir_out, pdf };
    }
    std::tuple<std::optional<Vector3f>, float>  operator()(const Transparent& bsdf) const {
        return {std::nullopt, 0};
    }
    const Vector3f &dir_in;
    const Intersection &vertex;
    const std::vector<Image> &texture_pool;
    const Vector2f &uv;
    //const float &w;
};
std::tuple<std::optional<Vector3f>, float>  sample_bsdf(const Material &material,
                                    const Vector3f &dir_in,
                                    const Intersection &inter,
                                    const std::vector<Image> &texture,
                                    const Vector2f &uv)
{
    return std::visit(sample_bsdf_op{dir_in, inter, texture, uv}, material);
}
struct pdf_sample_bsdf_op
{
    float operator()(const Lambertian &bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return 0.0f;
        }

        return fmax(dotProduct(vertex.normal, dir_out), 0.0f) / M_PI;
    }
    float operator()(const Phong &bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return 0.0f;
        }

        return fmax(dotProduct(vertex.normal.normalized(), dir_out.normalized()), 0.0f) / M_PI;
        return 0.5f / M_PI;
    }
    float operator()(const Transparent& bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return 0.0f;
        }

        return fmax(dotProduct(vertex.normal.normalized(), dir_out.normalized()), 0.0f) / M_PI;
        return 0.5f / M_PI;
    }
    float operator()(const Specular& bsdf) const
    {
        if (dotProduct(vertex.normal, dir_in) < 0 ||
            dotProduct(vertex.normal, dir_out) < 0)
        {
            return 0.0f;
        }

        return fmax(dotProduct(vertex.normal.normalized(), dir_out.normalized()), 0.0f) / M_PI;
        return 0.5f / M_PI;
    }
    const Vector3f &dir_in;
    const Vector3f &dir_out;
    const Intersection &vertex;
    const std::vector<Image> &texture_pool;
};
float pdf_sample_bsdf(const Material &material,
                      const Vector3f &dir_in,
                      const Vector3f &dir_out,
                      const Intersection &inter,
                      const std::vector<Image> &texture)
{
    return std::visit(pdf_sample_bsdf_op{dir_in, dir_out, inter, texture}, material);
}
