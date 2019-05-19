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
    LOG_DEBUG("Start create population with",
              " population_size = ", population_size,
              " mutation_ratio = ", mutation_ratio,
              " input_size = ", input_size,
              " and output_size = ", output_size);
    if (not (population_size >= 2) or not (input_size > 0) or not (output_size > 0) or not
             (mutation_ratio < 1.0) or isCreated_)
    {
        LOG_ERROR("Cannot create population with this properties",
                  (population_size >= 2 ? "" : "[invalid population size] "),
                  (mutation_ratio < 1.0 ? "" : "[mutation ratio above 1!] "),
                  (input_size > 0 ? "" : "[invalid input size] "),
                  (output_size > 0 ? "" : "[invalid output size] "),
                  (isCreated_ ? "[population already created -> reset() first]" : ""));
        return *this;
    }
    inputSize_ = input_size;
    failFunction_ = std::move(fail_function);
    for (auto i = 0; i < population_size; i++)
    {
        members_.push_back(
                    Member(static_cast<unsigned>(i+1), mutation_ratio,
                           input_size, output_size, fitness_function));
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

std::vector<std::pair<int, int>> Population::getRandomPairs(size_t max)
{
    std::vector<std::pair<int, int>> pairs;
    std::vector<int> membersIds(Generator::generate_int_vector(max));
    Generator::shuffle(membersIds);
    for (size_t i=0; i<membersIds.size()/2; i++)
    {
        const size_t first = 2*i;
        const size_t second = 2*i+1;
        pairs.push_back({membersIds[first], membersIds[second]});
    }
    return pairs;
}

Population::Members Population::copy_population_part_from_begin(double part)
{
    if (part > 1.0)
    {
        part = 1.0;
    }
    else if (part < 0.0)
    {
        part = 0.0;
    }
    part *= static_cast<double>(members_.size());
    int part_i = static_cast<int>(part);
    Members members_copy(members_.begin(), members_.begin() + part_i);
    return members_copy;
}

Population::Members Population::create_population_part(double part)
{
    if (part > 1.0)
    {
        part = 1.0;
    }
    else if (part < 0.0)
    {
        part = 0.0;
    }
    part *= static_cast<double>(members_.size());
    int part_i = static_cast<int>(part);
    Member member_prototype = members_.front();
    Members new_members;
    for (auto i = 0; i < part_i; i++)
    {
        new_members.emplace_back(Member(member_prototype).refresh_nn());
    }
    return new_members;
}

void Population::changeIdsAfterMixing()
{
    const unsigned FIRST_ID = 1;
    unsigned id = FIRST_ID;
    for (auto& member : members_)
    {
        member.id = id;
        ++id;
    }
    LOG_DEBUG("Created new id range for members in range ", FIRST_ID, " to ", id-1);
}

void Population::mixMembers()
{
    const auto POPULATION_PART_TO_MIXING = 0.3;
    const auto NEW_PART_OF_POPULATION = 0.2;
    sortMembersByFitness();
    const auto begin_size = members_.size();
    Members part_copy = copy_population_part_from_begin(POPULATION_PART_TO_MIXING);
    Members part_new = create_population_part(NEW_PART_OF_POPULATION);
    if (part_copy.empty())
    {
        LOG_INFO("No members to copy and mixing, increase population size or "
                 "change POPULATION_PART_TO_MIXING [0.0-1.0]");
        return;
    }

    const auto randoms = getRandomPairs(part_copy.size());
    for (const auto& rpair : randoms)
    {
        const auto f = static_cast<unsigned>(rpair.first);
        const auto s = static_cast<unsigned>(rpair.second);
        LOG_DEBUG("Mixing members [", f, " and ", s, "]");
        part_copy[f].mix(part_copy[s]);
    }
    const int new_part_size = part_new.size();
    const int copy_part_size = part_copy.size();
    const int both_part_size = new_part_size + copy_part_size;
    std::swap_ranges(part_new.begin(), part_new.end(),
                     members_.end() - both_part_size);
    std::swap_ranges(part_copy.begin(), part_copy.end(),
                     members_.end() - copy_part_size);
    Member::restart_members_fitness(members_);
    if (std::any_of(members_.begin(), members_.end(), [](const Member& m)
            { return m.fitness() > 0.01; }))
    {
        LOG_ERROR("Members fitnesses has not been cleared");
        throw std::logic_error("Cannot finish members mixing with not zero equals member fitness");
    }
    changeIdsAfterMixing();
    const auto end_size = members_.size();
    if (begin_size != end_size)
    {
        LOG_ERROR("New population size is different than previous! ",
                  "Previous: ", begin_size, " New: ", end_size);
        throw std::runtime_error("Invalid population size");
    }
    LOG_INFO("Mix in this generation done correctly, new population size is equal previous");
}

Population& Population::train_routine(
        const std::vector<Neurons>& examples,
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

Population& Population::decide(const Neurons& input_data)
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

Population& Population::one_member_prediction(
        Member& member,
        const Neurons& input_data)
{
    RETURN_IF_NOT_CREATED
    if (inputSize_ != input_data.size())
    {
        LOG_ERROR("Invalid input size. Expected: ", inputSize_,
                  " current: ", input_data.size());
        return *this;
    }
    if (not member.isDead())
    {
        LOG_DEBUG("Member not dead, train");
        member.train(input_data);
        if (failFunction_(member))
        {
            member.isDead(true);
        }
    }
    return *this;
}

Population& Population::unknown_member_prediction(
        Member& member,
        const Neurons& input_data)
{
    if (member.input_size() != input_data.size())
    {
        LOG_ERROR("Invalid input size. Expected: ", member.input_size(),
                  " current: ", input_data.size());
        return *this;
    }
    if (not member.isDead())
    {
        LOG_DEBUG("Member not dead, get prediction");
        member.train(input_data);
        if (failFunction_(member))
        {
            member.isDead(true);
            LOG_DEBUG("Member died");
        }
    }
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

Member& Population::getMember(unsigned id)
{
    if (not isCreated_)
    {
        LOG_ERROR("Population uninitialized. Cannot get member");
        throw std::runtime_error("Uninitialized population");
    } else if (members_.empty())
    {
        LOG_ERROR("No members in population. Cannot get member");
        throw std::runtime_error("No members in population");
    }
    if (id == 0) ++id;
    auto it = std::find_if(members_.begin(), members_.end(),
                           [id](const auto& member)
                           { return id == member.id; });
    if (it != members_.end())
    {
        LOG_DEBUG("Get member with id [", id, "]");
        return *it;
    }
    LOG_ERROR("No member with this id [", id, "]!");
    throw std::runtime_error("Wrong member id number");
}

Member& Population::getNext(const Member& member)
{
    if (not isCreated_)
    {
        LOG_ERROR("Population uninitialized. Cannot get member");
        throw std::runtime_error("Uninitialized population");
    } else if (members_.empty())
    {
        LOG_ERROR("No members in population. Cannot get member");
        throw std::runtime_error("No members in population");
    }
    unsigned nextId = member.id + 1;
    auto it = std::find_if(members_.begin(), members_.end(),
                           [nextId](const auto& member)
                           { return nextId == member.id; });
    if (it != members_.end())
    {
        return *it;
    }
    return members_.front();
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

unsigned Population::generation() const
{
    return generation_;
}
