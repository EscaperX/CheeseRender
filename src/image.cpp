#include "image.h"
#include <iostream>
#include <string>
#include "tinyexr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(const char *filename)
{
    auto components_per_pixel = bytes_per_pixel;

    stbi_set_flip_vertically_on_load(true);

    char buff[1024];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, filename);

    data = stbi_load(buff, &width, &height, &components_per_pixel, components_per_pixel);

    if (!data)
    {
        // std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

void imwrite(Image &img, std::string filename)
{
    int w = img.width, h = img.height, channels_num = 3;

    stbi_write_png(filename.c_str(), w, h, channels_num, img.data, 0);
}

Image imread(std::string filename)
{
    return Image(filename.c_str());
}

void imwrite_exr(Imagef &img, std::string filename)
{
    auto *buffer = new float[3 * img.width * img.height];
    int cnt = 0;
    for (int j = 0; j < img.height; j++)
        for (int i = 0; i < img.width; i++)
        {
            buffer[3 * cnt + 0] = img.at(i, j).x;
            buffer[3 * cnt + 1] = img.at(i, j).y;
            buffer[3 * cnt + 2] = img.at(i, j).z;
            cnt++;
        }
    SaveEXR(buffer, img.width, img.height, 3, 0, filename.c_str(), nullptr);
    std::cout << "\nImage saved to ./" << filename << std::endl;
}
