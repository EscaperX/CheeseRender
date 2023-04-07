#include "texture.h"
struct eval_texture_op
{
    Vector3f operator()(const ConstantTexture &t) const;
    Vector3f operator()(const ImageTexture &t) const;
    Vector3f operator()(const CheckerboardTexture &t) const;

    const Vector2f &uv;
    // const Real &footprint;
    const std::vector<Image> &pool;
};
Vector3f eval_texture_op::operator()(const ConstantTexture &t) const
{
    return t.value;
}
Vector3f eval_texture_op::operator()(const ImageTexture &t) const
{
    return pool[t.texture_id].at(uv.x, uv.y);
}
Vector3f eval_texture_op::operator()(const CheckerboardTexture &t) const
{
    int x = 2 * ((int)(uv.x * 2) % 2) - 1,
        y = 2 * ((int)(uv.y * 2) % 2) - 1;

    if (x * y == 1)
    {
        return t.color0;
    }
    else
    {
        return t.color1;
    }
}
Vector3f eval_texture(const Texture &texture, const Vector2f &uv, const std::vector<Image> &pool)
{
    return std::visit(eval_texture_op{uv, pool}, texture);
}
