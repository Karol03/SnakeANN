#include <algorithm>
#include <ctime>

#include "generator.hpp"
#include "logger.hpp"

std::random_device Generator::rd;
std::mt19937 Generator::mt_gen(rd());

void Generator::initialize()
{
    static bool initialize = false;
    if (not initialize)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        initialize = true;
    }
}

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
    std::generate(v.begin(), v.end(), get_random);
    std::swap(v, vec);
}

double Generator::get_random()
{
    return double(rand()) / (double(RAND_MAX) + 1.0) - 0.5;
}

int Generator::get_random_int(int min, int max)
{
    return (rand()+min)%(max-min);
}

void Generator::shuffle(std::vector<int>& vec)
{
    std::shuffle(vec.begin(), vec.end(), mt_gen);
}
