#include <algorithm>

#include "generator.hpp"

const int MINMUM_INT     = 0;
const int MAXIMUM_INT    = 1000;
const double LOWER_BOUND = -0.5;
const double UPPER_BOUND = 0.5;

Generator::Generator()
{}

std::random_device Generator::rd;
std::default_random_engine Generator::re(rd());
std::uniform_real_distribution<double> Generator::unif(LOWER_BOUND, UPPER_BOUND);
std::uniform_int_distribution<int> Generator::dist(MINMUM_INT, MAXIMUM_INT);

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
    std::generate(v.begin(), v.end(), [&]() { return unif(re); });
    std::swap(v, vec);
}

double Generator::get_random()
{
    return unif(re);
}

int Generator::get_random_int(int min, int max)
{
    return (dist(re)+min)%(max-min);
}

void Generator::shuffle(std::vector<int>& vec)
{
    std::shuffle(vec.begin(), vec.end(), re);
}
