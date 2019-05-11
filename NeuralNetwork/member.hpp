#pragma once
#include <functional>
#include <vector>

#include "neuralnetworkhandler.hpp"

class Member
{
public:
    struct Output
    {
        int number;
        double accuracy;
    };

    Member();
    Member(double mutation_ratio, size_t input_size, size_t output_size,
           std::function<double(const Member&)> fitness_function);

    Member& initialize(double mutation_ratio, size_t input_size, size_t output_size,
                       std::function<double(const Member&)> fitness_function);
    Member& train(const std::vector<int>& input_data);
    NeuralNetwork::Output prediction() const;
    double fitness() const;
    bool isDead() const;
    void isDead(bool isdead);
    void mix(Member& member);

private:
    bool isInitialized() const;

    bool isDead_;
    bool isInitialized_;
    double mutationRatio_;
    double fitness_;
    size_t inputSize_;
    size_t outputSize_;
    std::function<double(const Member&)> fitnessFunction_;
    NeuralNetworkHandler ann_;
};
