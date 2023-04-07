#ifndef CHEESE_GLTEXTURE_H
#define CHEESE_GLTEXTURE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "image.h"
using tex_handle = std::unique_ptr<uint8_t[], void (*)(void *)>;

class GLTexture
{
public:
    GLTexture() = default;
    GLTexture(const std::string &textureName) : mTextureName(textureName), mTextureId(0) {}
    ~GLTexture() noexcept
    {
        if (mTextureId)
            glDeleteTextures(1, &mTextureId);
    }

    GLuint texture() const { return mTextureId; }

    tex_handle load_file(const std::string &fileName);
    tex_handle load_data(uint8_t *data, int w, int h);
    tex_handle load_image(const Image &image);

private:
    std::string mTextureName;
    GLuint mTextureId;
};

#endif