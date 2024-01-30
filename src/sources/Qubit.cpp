#include "../headers/Qubit.hpp"

Qubit::Qubit() {
    alpha = {1, 0};
    beta  = {0, 0};
}

int Qubit::measure() {
    double alpha_probability = std::norm(alpha);
    double random = (double)std::rand() / RAND_MAX;
    if (random < alpha_probability) {
        std::cout << "    cur qubit:\n"; 
        dbg_log_qubit();
        alpha = {1, 0};
        beta  = {0, 0};
        return 0;
    } else {
        std::cout << "    cur qubit:\n"; 
        dbg_log_qubit();
        alpha = {0, 0};
        beta  = {1, 0};
        return 1;
    }
}

void Qubit::reset() {
    alpha = {1, 0};
    beta  = {0, 0};
}

std::complex<double> Qubit::get_alpha() {
    return alpha;
}

std::complex<double> Qubit::get_beta() {
    return beta;
}

void Qubit::set_alpha(std::complex<double> alpha_tmp) {
    alpha = alpha_tmp;
}

void Qubit::set_beta(std::complex<double> beta_tmp) {
    beta = beta_tmp;
}

void Qubit::dbg_log_qubit() {
    std::cout << "    a: " << alpha << " b: " << beta << "\n";
}

