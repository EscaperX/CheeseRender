#pragma once

#include "math.h"

#include <vector>



template <typename T>
class Image {
public:
    Image () {}
    Image (int w, int h):width(w), height(h) {
        data.resize(w * h);  
        memset(data.data(), 0, sizeof(T) * data.size());
    }
    T & at(int x) {
        return data[x];
    } 
    const T& at(int x) const {
        return data[x];
    }
    // x : row
    // y : col
    T & at(int row, int col) {
        return data[row * width + col]; 
    }
    const T & at(int row, int col) const {
        return data[row * width + col];
    }
private:
    std::vector<T> data;
    int width, height;
};

