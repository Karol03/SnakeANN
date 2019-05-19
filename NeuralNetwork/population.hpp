#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "member.hpp"

class Population
{
public:
    using Members = std::vector<Member>;

    Population();

    Population& create(int population_size, double mutation_ratio,
                       size_t input_size, size_t output_size,
                       std::function<double(const Member&)> fitness_function,
                       std::function<bool(const Member&)> fail_function);
    Population& decide(const Neurons& input_data);
    Population& one_member_prediction(Member& member,
                                      const Neurons& input_data);
    Population& evolve();
    Population& forceEvolve();
    Population& getBestMember(std::unique_ptr<Member>&);
    Population& reset();
    Population& train_routine(
            const std::vector<Neurons>& examples,
            std::function<void(Population&)> function = {});
    Population& unknown_member_prediction(
            Member& member,
            const Neurons& input_data);

    Member& getMember(unsigned id);
    Member& getNext(const Member& member);

    void setBestPlayer(std::unique_ptr<Member>& m) { m.swap(bestPlayer_); }
    std::unique_ptr<Member>& best_player() { return bestPlayer_; }

    int livingMembers() const;
    unsigned generation() const;

private:
    Members copy_population_part_from_begin(double part);
    Members create_population_part(double part);
    void sortMembersByFitness();
    void mixMembers();
    void changeIdsAfterMixing();
    bool isCreated() const;
    std::vector<std::pair<int, int>> getRandomPairs(size_t max);

    bool isCreated_;
    size_t inputSize_;
    unsigned generation_;
    std::function<bool(const Member&)> failFunction_;
    Members members_;
    std::unique_ptr<Member> bestPlayer_;
};
