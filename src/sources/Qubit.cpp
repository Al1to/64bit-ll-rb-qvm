#include "../headers/Qubit.hpp"

Qubit::Qubit() {
    alpha = {1, 0};
    beta  = {0, 0};
}

int Qubit::measure() {
    double alpha_probability = std::norm(alpha);
    double random = (double)std::rand() / RAND_MAX;
    if (random < alpha_probability) {
        alpha = {1, 0};
        beta  = {0, 0};
        dbg_log_qubit();
        return 0;
    } else {
        alpha = {0, 0};
        beta  = {1, 0};
        dbg_log_qubit();
        return 1;
    }
}

void Qubit::reset() {
    alpha = {1, 0};
    beta  = {0, 0};
}

void Qubit::dbg_log_qubit() {
    std::cout << "    a: " << alpha << " b: " << beta << "\n";
}

