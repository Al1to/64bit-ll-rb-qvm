#pragma once

#include <complex>

#include "../utils.hpp"

struct Qubit {
    std::complex<double> alpha;
    std::complex<double> beta;

    int measure();
};
