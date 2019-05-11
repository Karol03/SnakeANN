#include "generator.hpp"

#include <algorithm>
#include <random>

Generator::Generator()
{}

std::vector<int> Generator::generate_int_vector(size_t size)
{
    const int START_FROM_NUMBER = 0;
    std::vector<int> vec(size);
    std::iota(vec.begin(), vec.end(), START_FROM_NUMBER);
    return vec;
}

void Generator::generate_random_vector(std::vector<double>& vec,
                                       std::size_t size)
{
    std::vector<double> v(size);
    double lower_bound = -0.5;
    double upper_bound = 0.5;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    auto gen = [&re, &unif]()
    {
        return unif(re);
    };
    std::generate(v.begin(), v.end(), gen);
    std::swap(v, vec);
}

double Generator::get_random(double min, double max)
{
    std::uniform_real_distribution<double> unif(min, max);
    std::default_random_engine re;
    return unif(re);
}

void Generator::shuffle(std::vector<int>& vec)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
}
