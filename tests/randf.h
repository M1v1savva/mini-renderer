#include <fstream>
#include <random>

inline float randf() {
    const int TEST_SEED = 42;
    static std::mt19937 rng(TEST_SEED);
    static std::uniform_real_distribution<float> dist(-10.f, 10.f);
    return dist(rng);
}