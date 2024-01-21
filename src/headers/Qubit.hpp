#pragma once

#include <iostream>
#include <complex>

#include "../utils.hpp"

struct Qubit {
    std::complex<double> alpha;
    std::complex<double> beta;

    Qubit();

    int measure();

    void reset();

    void dbg_log_qubit();
};
