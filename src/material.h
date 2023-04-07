#ifndef CHEESE_MATERIAL_H
#define CHEESE_MATERIAL_H

#include "cheese.h"
#include "texture.h"

struct Intersection;

struct Lambertian
{
    Texture reflectance;
    std::optional<Texture> diffuse_map;
};

struct Phong
{
    Texture diffuse;
    Texture specular;
    Texture spec_exponent;
    Texture transparent;
    Texture ior;
    std::optional<Texture> diffuse_map;
};
struct Transparent {
    Texture transparent;
    Texture Ni;
};

struct Specular {
    Texture specular;
    Texture spec_exponent;
};

using Material = std::variant<Lambertian, Phong, Transparent, Specular>;

float luminance(const Material& material, const Intersection& inter, const std::vector<Image>& texture);

Vector3f eval(const Material &material,
              const Vector3f &dir_in,
              const Vector3f &dir_out,
              const Intersection &inter,
              const std::vector<Image> &texture);

std::tuple<std::optional<Vector3f>, float> sample_bsdf(const Material& material,
    const Vector3f& dir_in,
    const Intersection& inter,
    const std::vector<Image>& texture,
    const Vector2f& uv);
                                    //const float &w);
float pdf_sample_bsdf(const Material &material,
                      const Vector3f &dir_in,
                      const Vector3f &dir_out,
                      const Intersection &inter,
                      const std::vector<Image> &texture);
#endif
