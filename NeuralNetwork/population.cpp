#include <algorithm>
#include <utility>

#include "utilities/generator.hpp"
#include "utilities/logger.hpp"
#include "population.hpp"

#define RETURN_IF_NOT_CREATED if(not isCreated()) return *this;

Population::Population()
    : isCreated_(false)
    , generation_(1)
{}

Population& Population::create(int population_size, double mutation_ratio,
                               size_t input_size, size_t output_size,
                               std::function<double(const Member&)> fitness_function,
                               std::function<bool(const Member&)> fail_function)
{
    if (not (population_size >= 2 and input_size > 0 and output_size > 0) and not isCreated_)
    {
        LOG_ERROR("Cannot create population with this properties",
                  (population_size > 2 ? "" : "[invalid population size] "),
                  (input_size > 0 ? "" : "[invalid input size] "),
                  (output_size > 0 ? "" : "[invalid output size] "),
                  (isCreated_ ? "[population already created -> reset() first]" : ""));
        return *this;
    }
    inputSize_ = input_size;
    failFunction_ = std::move(fail_function);
    for (auto i = 0; i < population_size; i++)
    {
        members_.push_back(Member(mutation_ratio, input_size, output_size, fitness_function));
    }
    LOG_INFO("Creating done. Population have [", population_size, "] new members");
    isCreated_ = true;
    return *this;
}

Population& Population::evolve()
{
    RETURN_IF_NOT_CREATED
    if (not std::all_of(members_.begin(), members_.end(), [](const Member& m) { return m.isDead(); }))
    {
        LOG_INFO("Cannot evolve. Not all members died");
        return *this;
    }
    forceEvolve();
    return *this;
}

Population& Population::forceEvolve()
{
    RETURN_IF_NOT_CREATED
    for (auto& member : members_)
    {
        member.isDead(false);
    }
    mixMembers();
    ++generation_;
    LOG_INFO("Go to new generation Gen: ", generation_);
    return *this;
}

std::vector<std::pair<int, int>> Population::getRandomPairs()
{
    std::vector<std::pair<int, int>> pairs;
    std::vector<int> membersIds(Generator::generate_int_vector(members_.size()));
    Generator::shuffle(membersIds);
    for (size_t i=0; i<membersIds.size()/2; i++)
    {
        const size_t first = 2*i;
        const size_t second = 2*i+1;
        pairs.push_back({membersIds[first], membersIds[second]});
    }
    return pairs;
}

void Population::mixMembers()
{
    const auto randoms = getRandomPairs();
    for (const auto& rpair : randoms)
    {
        const auto f = static_cast<unsigned>(rpair.first);
        const auto s = static_cast<unsigned>(rpair.second);
        LOG_DEBUG("Mixing members [", f, " and ", s, "]");
        members_[f].mix(members_[s]);
    }
    LOG_INFO("Mix in this generation done");
}

Population& Population::train_routine(
        const std::vector<std::vector<int>>& examples,
        std::function<void(Population&)> function)
{
    RETURN_IF_NOT_CREATED
    LOG_INFO("Train routine with [", examples.size(), "] examples and ",
             (function ? "with" : "without"), " function starting...");
    unsigned i = 0;
    for (const auto& example : examples)
    {
        decide(example).forceEvolve();
        LOG_DEBUG("Example number [", i++, "]");
        if (function)
        {
            function(*this);
        }
    }
    LOG_INFO("Training done. All [", examples.size(), "] examples was trained");
    return *this;
}

Population& Population::decide(const std::vector<int>& input_data)
{
    RETURN_IF_NOT_CREATED
    if (inputSize_ != input_data.size())
    {
        LOG_ERROR("Invalid input size. Expected: ", inputSize_,
                  " current: ", input_data.size());
        return *this;
    }
    for (auto& member : members_)
    {
        if (not member.isDead())
        {
            member.train(input_data);
            if (failFunction_(member))
            {
                member.isDead(true);
            }
        }
    }
    LOG_INFO("All members were predicted decision");
    return *this;
}

int Population::livingMembers() const
{
    return std::count_if(members_.begin(), members_.end(), [](const Member& m)
        { return not m.isDead(); });
}

Population& Population::getBestMember(std::unique_ptr<Member>& member_)
{
    RETURN_IF_NOT_CREATED
    sortMembersByFitness();
    member_.reset(new Member(members_[0]));
    return *this;
}

Population& Population::reset()
{
    if (isCreated_)
    {
        LOG_DEBUG("No need to reset. Popultation was not created yet");
        return *this;
    }
    members_.clear();
    isCreated_ = false;
    generation_ = 1;
    LOG_INFO("Population cleared. Start from generation [", generation_, "]");
    return *this;
}

void Population::sortMembersByFitness()
{
    std::sort(members_.begin(), members_.end(), [](const Member& m1, const Member& m2)
        { return m1.fitness() > m2.fitness(); });
}

bool Population::isCreated() const
{
    if (not isCreated_)
    {
        LOG_ERROR("Create population first");
        return false;
    }
    return true;
}
