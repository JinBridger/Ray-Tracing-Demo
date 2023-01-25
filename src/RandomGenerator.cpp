#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() : m_distribution(0.0, 1.0), m_randomDevice(), m_mtGen(m_randomDevice()) {}

float RandomGenerator::generate() {
    return m_distribution(m_mtGen);
}