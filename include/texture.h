#ifndef CHEESE_TEXTURE_H
#define CHEESE_TEXTURE_H

#include "cheese.h"
#include "image.h"
struct ConstantTexture
{
    ConstantTexture(float x = 0) : value(Vector3f{x, x, x}) {}
    ConstantTexture(Vector3f x) : value(x) {}
    Vector3f value;
};

struct ImageTexture
{
    int texture_id;
    ImageTexture(int id) : texture_id(id) {}
};

struct CheckerboardTexture
{
    Vector3f color0, color1;
    float uscale, vscale;
    float uoffset, voffset;
};

using Texture = std::variant<ConstantTexture, ImageTexture, CheckerboardTexture>;

Vector3f eval_texture(const Texture &texture, const Vector2f &uv, const std::vector<Image> &pool);
#endif
