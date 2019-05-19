#include <utility>

#include "member.hpp"
#include "utilities/logger.hpp"

#define RETURN_IF_NOT_INITIALIZED if(not isInitialized()) { return *this; }

Member::Member(unsigned id)
    : id(id)
    , isDead_(false)
    , isInitialized_(false)
    , fitness_(0.0)
{}

Member::Member(unsigned id, double mutation_ratio, size_t input_size, size_t output_size,
               std::function<double(const Member&)> fitness_function)
    : id(id)
    , isDead_(false)
    , isInitialized_(false)
    , fitness_(0.0)
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
    inputSize_ = input_size;
    outputSize_ = output_size;
    ann_.create(input_size, output_size, NeuralNetworkHandler::Type::OneHiddenLayer);
    isInitialized_ = true;
    LOG_DEBUG("Initialization complete");
    return *this;
}

Member& Member::refresh_nn()
{
    NeuralNetworkHandler new_ann;
    new_ann.create(inputSize_, outputSize_, NeuralNetworkHandler::Type::OneHiddenLayer);
    std::swap(ann_, new_ann);
    return *this;
}

Member& Member::train(const Neurons& input_data)
{
    RETURN_IF_NOT_INITIALIZED
    ann_.train(input_data);
    fitness_ += fitnessFunction_(*this);
    LOG_DEBUG("Memeber train finished with fitness [", fitness_, "]");
    return *this;
}

Output Member::prediction() const
{
    return ann_.prediction();
}

void Member::mix(Member& member)
{
    const double percentageFirstNNPart = 0.3;
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
        LOG_ERROR("Member is not initialized");
        return false;
    }
    return true;
}

Member& Member::mutation_ratio(double mutation)
{
    mutationRatio_ = mutation;
    return *this;
}

Member& Member::input_size(size_t size)
{
    inputSize_ = size;
    return *this;
}

Member& Member::output_size(size_t size)
{
    outputSize_ = size;
    return *this;
}

Member& Member::fitness_function(std::function<double(const Member&)> function)
{
    fitnessFunction_ = function;
    return *this;
}

Member& Member::initialized(bool initialized)
{
    isInitialized_ = initialized;
    return *this;
}
