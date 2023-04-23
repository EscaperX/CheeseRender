#ifndef CHEESE_RENDER_H
#define CHEESE_RENDER_H

#include "cheese.h"

#include "image.h"
#include "scene.h"
#include <vector>

struct Render_Option
{
    const static Render_Option &instance()
    {
        static Render_Option opt;
        return opt;
    }
    int thread_num = 20;
    int spp = 32;
    int depth = 10;
    bool gbuffer = true;
    float threshold = 0.8;
    bool sample_light = true;
    int min_depth = 5;
};

std::vector<Imagef> render(const Scene &scene);

#endif