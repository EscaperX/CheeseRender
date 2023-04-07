#ifndef CHEESE_IMAGE_H
#define CHEESE_IMAGE_H
#include "cheese.h"
#include <vector>
struct Image
{
    const static int bytes_per_pixel = 3;

    Image() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    Image(const char* filename);
    Image(Image& img) : width(img.width), height(img.height), bytes_per_scanline(img.bytes_per_scanline) {
        data = img.data;
        img.data = nullptr;
    }

    ~Image()
    {

        delete data;
    }

    Vector3f at(float u, float v) const 
    {
        if (data == nullptr)
            return Vector3f(-1, -1, -1);

        u = u - floor(u);
        v = v - floor(v);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        Vector3f linear_color;
        linear_color.x = pow(color_scale * pixel[0], 2.2);
        linear_color.y = pow(color_scale * pixel[1], 2.2);
        linear_color.z = pow(color_scale * pixel[2], 2.2);

        return linear_color;
    }

    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};
struct Imagef
{

    Imagef(){}

    Imagef(int width, int height):width(width), height(height) {
        data.resize(width * height);
    }
    ~Imagef()
    {
        data.clear();
    }

    Vector3f& at(int i, int j)
    {
        if (data.size() < j * width + i)
            return Vector3f(-1, -1, -1);

        if (i >= width)
            i = width - 1;
        if (j >= height)
            j = height - 1;

        return data[j * width + i];

    }

    std::vector<Vector3f> data;
    int width, height;
};



void imwrite(Image &img, std::string filename);
void imwrite_exr(Imagef &img, std::string filename);
#endif
