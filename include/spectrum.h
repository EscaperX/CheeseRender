#pragma once

#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include "headers.h"

template <typename T, int N>
class TSpectrum {

public:
    
    const static int dim = N;

private:
    T v[N];
};

class RGBSpectrum : public TSpectrum<float, 3> {

};

#endif
