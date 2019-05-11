#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "member.hpp"

class Population
{
public:
    Population();

    Population& create(int population_size, double mutation_ratio,
                       size_t input_size, size_t output_size,
                       std::function<double(const Member&)> fitness_function,
                       std::function<bool(const Member&)> fail_function);
    Population& decide(const std::vector<int>& input_data);
    Population& evolve();
    Population& forceEvolve();
    Population& getBestMember(std::unique_ptr<Member>&);
    Population& reset();
    Population& train_routine(
            const std::vector<std::vector<int>>& examples,
            std::function<void(Population&)> function = {});

    int livingMembers() const;
    unsigned generation() const;
private:
    void sortMembersByFitness();
    void mixMembers();
    bool isCreated() const;
    std::vector<std::pair<int, int>> getRandomPairs();

    bool isCreated_;
    size_t inputSize_;
    unsigned generation_;
    std::function<bool(const Member&)> failFunction_;
    std::vector<Member> members_;
};