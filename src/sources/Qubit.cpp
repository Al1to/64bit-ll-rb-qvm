#include "../headers/Qubit.hpp"

int Qubit::measure() {
    double alpha_probability = std::norm(alpha);
    double random = (double)std::rand() / RAND_MAX;
    if (random < alpha_probability) {
        alpha = {1, 0};
        beta = {0, 0};
        return 0;
    } else {
        alpha = {0, 0};
        beta = {1, 0};
        return 1;
    }
}
