#pragma once
#include <functional>
#include <vector>

#include "neuralnetworkhandler.hpp"

class Member
{
public:
    Member(unsigned id);
    Member(unsigned id, double mutation_ratio, size_t input_size, size_t output_size,
           std::function<double(const Member&)> fitness_function);

    Member& initialize(double mutation_ratio, size_t input_size, size_t output_size,
                       std::function<double(const Member&)> fitness_function);
    Member& train(const Neurons& input_data);
    Member& mutation_ratio(double mutation);
    Member& input_size(size_t size);
    Member& output_size(size_t size);
    Member& fitness_function(std::function<double(const Member&)> function);
    Member& initialized(bool initialized);
    Member& refresh_nn();

    Output prediction() const;
    double fitness() const;
    void mix(Member& member);
    bool isDead() const { return isDead_; }
    void isDead(bool isdead) { isDead_ = isdead; }
    const NeuralNetworkHandler& handler() const { return ann_; }
    void setHandler(NeuralNetworkHandler& ann) { std::swap(ann, ann_); }
    size_t input_size() { return inputSize_; }
    void reset_fitness() { fitness_ = 0.0; }

    static void restart_members_fitness(std::vector<Member>& m)
    { std::for_each(m.begin(), m.end(), [](Member& m) { m.fitness_ = 0.0; }); }

    bool initialized() const { return isInitialized_; }

    unsigned id;
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
