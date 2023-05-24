#ifndef CHEESE_CHEESE_H
#define CHEESE_CHEESE_H

#include "vector.h"
#include "log.h"
#include <assert.h>
#include <variant>
#include <utility>
#include <optional>
#include <memory>
const float M_PI = 3.14159265358979323846f;
const float INV_PI = 1.0f / M_PI;
const float INV_2_PI = 1.0f / M_PI / 2.0f;
const float EPSILON = 0.00001f;
const float INF = std::numeric_limits<float>::infinity();
#define deg2rad(deg) ((deg)*M_PI / 180.0f)

// Stolen from https://github.com/halide/Halide/blob/c6529edb23b9fab8b406b28a4f9ea05b08f81cfe/src/Util.cpp#L253
inline float clamp(float x, float ub = 1.0f, float lb = 0.0f)
{
    if (x > ub)
        return ub;
    if (x < lb)
        return lb;
    return x;
}
inline bool ends_with(const std::string &str, const std::string &suffix)
{
    if (str.size() < suffix.size())
    {
        return false;
    }
    size_t off = str.size() - suffix.size();
    for (size_t i = 0; i < suffix.size(); i++)
    {
        if (str[off + i] != suffix[i])
        {
            return false;
        }
    }
    return true;
}

#endif
