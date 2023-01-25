#pragma once

#include <random>

class RandomGenerator {
public:
    RandomGenerator();

    float generate();

private:
    std::uniform_real_distribution<> m_distribution;
    std::random_device               m_randomDevice;
    std::mt19937                     m_mtGen;
};