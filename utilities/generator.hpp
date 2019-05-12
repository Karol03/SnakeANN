#pragma once

#include <vector>
#include <random>

class Generator
{
    Generator() = default;
public:
    static void initialize();
    static std::vector<int> generate_int_vector(std::size_t size);
    static void generate_random_vector(std::vector<double>& vec,
                                       std::size_t size);
    static double get_random();
    static int get_random_int(int min, int max);
    static void shuffle(std::vector<int>& vec);

private:
    static std::random_device rd;
  //  static std::default_random_engine re;
    static std::mt19937 mt_gen;
  //  static std::uniform_real_distribution<double> unif;
  //  static std::uniform_int_distribution<int> dist;
};
