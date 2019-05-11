#include <utility>

#include "member.hpp"
#include "utilities/logger.hpp"

#define RETURN_IF_NOT_INITIALIZED if(not isInitialized()) { return *this; }

Member::Member()
    : isInitialized_(false)
{}

Member::Member(double mutation_ratio, size_t input_size, size_t output_size,
               std::function<double(const Member&)> fitness_function)
{
    initialize(mutation_ratio, input_size, output_size, fitness_function);
}

Member& Member::initialize(double mutation_ratio, size_t input_size, size_t output_size,
                   std::function<double(const Member&)> fitness_function)
{
    if (not (input_size > 0 and output_size > 0) and not isInitialized_)
    {
        LOG_ERROR("Member initialize failed ",
                  (input_size > 0 ? "" : "input size too small "),
                  (output_size > 0 ? "" : "output size too small "),
                  (isInitialized_ ? "member is already initialized" : ""));
        return *this;
    }

    mutationRatio_ = mutation_ratio;
    fitnessFunction_ = std::move(fitness_function);
    ann_.create(input_size, output_size, NeuralNetworkHandler::Type::OneHiddenLayer);
    isInitialized_ = true;
    LOG_DEBUG("Initialization complete");
    return *this;
}

Member& Member::train(const std::vector<int>& input_data)
{
    RETURN_IF_NOT_INITIALIZED
    ann_.train(input_data);
    fitness_ = fitnessFunction_(*this);
    LOG_DEBUG("Memeber train finished with fitness [", fitness_, "]");
    return *this;
}

NeuralNetwork::Output Member::prediction() const
{
    return ann_.prediction();
}

bool Member::isDead() const
{
    return isDead_;
}

void Member::isDead(bool isdead)
{
    isDead_ = isdead;
}

void Member::mix(Member& member)
{
    const double percentageFirstNNPart = 0.7;
    ann_.mixNN(member.ann_, percentageFirstNNPart, mutationRatio_);
}

double Member::fitness() const
{
    return fitness_;
}

bool Member::isInitialized() const
{
    if (not isInitialized_)
    {
        LOG_ERROR("Initialize member first");
        return false;
    }
    return true;
}
