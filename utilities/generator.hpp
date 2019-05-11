#pragma once

#include <vector>

class Generator
{
    Generator();
public:
    static std::vector<int> generate_int_vector(std::size_t size);
    static void generate_random_vector(std::vector<double>& vec,
                                       std::size_t size);
    static double get_random(double min, double max);
    static void shuffle(std::vector<int>& vec);
};
