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

    std::complex<double> get_alpha();

    std::complex<double> get_beta();

    void set_alpha(std::complex<double> alpha);

    void set_beta(std::complex<double> beta);

    void dbg_log_qubit();
};
