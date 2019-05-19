#include <cmath>
#include <memory>

#include "autocontroller.hpp"
#include "utilities/config.hpp"
#include "utilities/savemember.hpp"

namespace detail
{
void turn_off_window_and_logger()
{
    LOG_INFO("Turn off window drawing until population members learn.. ",
             "Logger set to ERROR_ONLY level");
    CONFIG::for_window::DRAW_WINDOW = false;
    CONFIG::for_control::DELAY_AFTER_DRAWING = false;
    CONFIG::for_control::DELAY_TIME = 0;
    logger::Logging log(logger::Logging::Error_only);

}

void turn_on_window_and_logger()
{
    LOG_INFO("Turn on window drawing. Logger set to Info");
    CONFIG::for_window::DRAW_WINDOW = true;
    CONFIG::for_control::DELAY_AFTER_DRAWING = true;
    CONFIG::for_control::DELAY_TIME = 100;
    logger::Logging log(logger::Logging::Info);
}

void print_best_member_stats(const unsigned generation,
                             const std::unique_ptr<Member>& m)
{
    logger::Logging log(logger::Logging::Info);
    LOG_INFO("Found best player in generation [", generation, "] with",
             " fitness [", m->fitness(), "/",
             CONFIG::for_autocontroller_correction::LOWER_BOUND, "]");
    logger::Logging log_2(logger::Logging::Error_only);
}

const char* chooseToPrint(int i)
{
    switch (i)
    {
    case 0: return "UP";
    case 1: return "RIGHT";
    case 2: return "DOWN";
    case 3: return "LEFT";
    default: return "UNKNOWN OPTION!";
    }
}

char chooseToKey(int i)
{
    switch (i)
    {
    case 0: return UP_KEY;
    case 1: return RIGHT_KEY;
    case 2: return DOWN_KEY;
    case 3: return LEFT_KEY;
    default:
        LOG_ERROR("Unknown key with value [", i, "]");
        throw std::runtime_error("Unknown key");
    }
}
}  // namespace detail

AutoController::AutoController(Stage& stage, bool& dead)
    : gameTheEnd_(dead)
    , isMemberCanPlay_(false)
    , stage_(stage)
    , current_id_(1)
{
    detail::turn_off_window_and_logger();
    isMemberCanPlay_ = SaveMember::isMemberCanPlay();
}

void AutoController::checkIfFoundBestPlayer()
{
    static double MEMBER_LOWER_BOUND = CONFIG::for_autocontroller_correction::LOWER_BOUND;
    static double fitness_ = 0.0;
    std::unique_ptr<Member> bestMember;
    population_.getBestMember(bestMember);
    detail::print_best_member_stats(population_.generation(), bestMember);
    if (bestMember->fitness() > MEMBER_LOWER_BOUND and
        bestMember->fitness() > fitness_)
    {
        fitness_ = bestMember->fitness();
        MEMBER_LOWER_BOUND *= 2;
        detail::turn_on_window_and_logger();
        LOG_INFO("Member got better fitness than lower bound");
        if (CONFIG::for_control::SAVE_BEST_SNAKE_TO_FILE)
        {
            LOG_INFO("Saving best player");
            SaveMember(bestMember).save();
        }
        else
        {
            LOG_INFO("Reset population and start best member");
            population_.reset();
            population_.setBestPlayer(bestMember);
        }
    }
}

int AutoController::play()
{
    if (isMemberCanPlay_ and CONFIG::for_control::LOAD_MEMBERS_FROM_FILE)
    {
        Member& member = SaveMember::getMemberFromFile();
        if (not member.initialized())
        {
            detail::turn_on_window_and_logger();
            population_.reset();
            const unsigned FIRST_MEMBER_ID = 1;
            const auto FITNESS_FUNCTION = [&](const Member& member)
            { return fitness_function(member); };
            member.mutation_ratio(CONFIG::for_genetic_algorithm::MUTATION_RATIO)
                  .fitness_function(FITNESS_FUNCTION)
                  .initialized(true);
            member.id = FIRST_MEMBER_ID;
            current_id_ = FIRST_MEMBER_ID;
            LOG_DEBUG("Member initialized after load from file");
        }
        return playMemberFromFile(member);
    }
    else if (population_.best_player())
    {
        return playMemberFromFile(*population_.best_player());
    }
    else
    {
        return trainPlayers();
    }
}

int AutoController::playMemberFromFile(Member& member)
{
    population_.unknown_member_prediction(member, getInputData());
    if (member.isDead())
    {
        LOG_INFO("Member died. Press any key to restart...");
        getchar();
        return NEW_GAME;
    }
    return detail::chooseToKey(member.prediction().choose);
}

int AutoController::trainPlayers()
{
    const unsigned FIRST_MEMBER_ID = 1;
    LOG_DEBUG("Decide next move");
    if (current_id_ > static_cast<unsigned>(POPULATION_SIZE))
    {
        current_id_ = FIRST_MEMBER_ID;
    }
    Member& current_member = population_.getMember(current_id_);

    if (current_member.fitness() >
            CONFIG::for_autocontroller_correction::UPPER_BOUND and
        stage_.getSnake().position().size() <
            CONFIG::for_autocontroller_correction::MINIMAL_LENGTH)
    {
        current_member.isDead(true);
        current_member.reset_fitness();
    }

    const int NO_ONE_STILL_ALIVE = 0;

    if (population_.livingMembers() == NO_ONE_STILL_ALIVE)
    {
        LOG_INFO("All members are dead. Restarting and create new generation");
        checkIfFoundBestPlayer();
        population_.evolve();
        current_id_ = FIRST_MEMBER_ID;
        LOG_INFO("Restart current_member_id to [", current_id_, "] and start new game");
        return NEW_GAME;
    }

    if (current_member.isDead())
    {
        LOG_DEBUG("Member [", current_id_, "] died. Get next and restart the game");
        ++current_id_;
        return NEW_GAME;
    }
    else
    {
        LOG_DEBUG("Member [", current_id_, "] get prediction");
        population_.one_member_prediction(current_member, getInputData());
    }
    LOG_DEBUG("Member prediction is: [",
              detail::chooseToPrint(current_member.prediction().choose),
              "] with accuracy: [", current_member.prediction().accuracy, "] ");
    return detail::chooseToKey(current_member.prediction().choose);
}

std::vector<double> AutoController::getInputData()
{
    std::vector<double> input_data(INPUT_SIZE);
    auto head = stage_.getSnake().getHead();
    auto body = stage_.getSnake().position();
    auto feed = stage_.getFeed().position();
    auto edge = stage_.edge();

    input_data[0] = angle_to_feed(head, feed);
    input_data[1] = distance_to_feed();
    input_data[2] = distance_to_wall(Direction::Up, head, edge);
    input_data[3] = distance_to_wall(Direction::Right, head, edge);
    input_data[4] = distance_to_wall(Direction::Down, head, edge);
    input_data[5] = distance_to_wall(Direction::Left, head, edge);

    input_data[6] = distance_to_wall(Direction::Up_Right, head, edge);
    input_data[7] = distance_to_wall(Direction::Up_Left, head, edge);
    input_data[8] = distance_to_wall(Direction::Down_Right, head, edge);
    input_data[9] = distance_to_wall(Direction::Down_Left, head, edge);

    input_data[10] = distance_to_snake(Direction::Up, head, body, edge);
    input_data[11] = distance_to_snake(Direction::Right, head, body, edge);
    input_data[12] = distance_to_snake(Direction::Down, head, body, edge);
    input_data[13] = distance_to_snake(Direction::Left, head, body, edge);

    input_data[14] = distance_to_snake(Direction::Up_Right, head, body, edge);
    input_data[15] = distance_to_snake(Direction::Up_Left, head, body, edge);
    input_data[16] = distance_to_snake(Direction::Down_Right, head, body, edge);
    input_data[17] = distance_to_snake(Direction::Down_Left, head, body, edge);
    return input_data;
}

double AutoController::angle_to_feed(sf::Vector2i head,
                                     sf::Vector2i feed)
{
    double x = static_cast<double>(feed.x - head.x);
    double y = static_cast<double>(feed.y - head.y);
    double fi = atan(y/x);
    LOG_DEBUG("Angle: fi = ", fi, "  result = ", fi);
    return fi;
}


double AutoController::distance_to_feed()
{
    double distance = distanceFromFeedToHead();
    LOG_DEBUG("Distance = ", distance);
    return distance;
}

double AutoController::distance_to_wall(Direction direction,
                                        sf::Vector2i head,
                                        sf::Vector2i stage_edge)
{
    double distance = 0.0;
    switch (direction)
    {
    case Direction::Up:
        while (head.y > 0)
        {
            --head.y;
            distance += 1;
        }
        LOG_DEBUG("Direction::Up distance = ", distance);
        return (distance/static_cast<double>(stage_edge.y));
    case Direction::Down:
        while (head.y+1 < stage_edge.y)
        {
            ++head.y;
            distance += 1;
        }
        LOG_DEBUG("Direction::Down distance = ", distance);
        return (distance/static_cast<double>(stage_edge.y));
    case Direction::Left:
        while (head.x > 0)
        {
            --head.x;
            distance += 1;
        }
        LOG_DEBUG("Direction::Left distance = ", distance);
        return (distance/static_cast<double>(stage_edge.x));
    case Direction::Right:
        while (head.x+1 < stage_edge.x)
        {
            ++head.x;
            distance += 1;
        }
        LOG_DEBUG("Direction::Right distance = ", distance);
        return (distance/static_cast<double>(stage_edge.x));
    case Direction::Up_Right:
        while (head.y > 0 and head.x+1 < stage_edge.x)
        {
            --head.y;
            ++head.x;
            distance += std::sqrt(2.0);
        }
        LOG_DEBUG("Direction::Up_Right distance = ", distance);
        return (distance/(std::sqrt(static_cast<double>(stage_edge.y^2) +
                          static_cast<double>(stage_edge.x^2))));
    case Direction::Up_Left:
        while (head.y > 0 and head.x > 0)
        {
            --head.y;
            --head.x;
            distance += std::sqrt(2.0);
        }
        LOG_DEBUG("Direction::Up_Left distance = ", distance);
        return (distance/(std::sqrt(static_cast<double>(stage_edge.y^2) +
                          static_cast<double>(stage_edge.x^2))));
    case Direction::Down_Right:
        while (head.y+1 < stage_edge.y and head.x > 0)
        {
            ++head.y;
            --head.x;
            distance += std::sqrt(2.0);
        }
        LOG_DEBUG("Direction::Down_Right distance = ", distance);
        return (distance/(std::sqrt(static_cast<double>(stage_edge.y^2) +
                          static_cast<double>(stage_edge.x^2))));
    case Direction::Down_Left:
        while (head.y+1 < stage_edge.y and head.x+1 < stage_edge.x)
        {
            ++head.y;
            ++head.x;
            distance += std::sqrt(2.0);
        }
        LOG_DEBUG("Direction::Down_Left distance = ", distance);
        return (distance/(std::sqrt(static_cast<double>(stage_edge.y^2) +
                          static_cast<double>(stage_edge.x^2))));
    }
    return 0.0;
}

bool AutoController::isSnakeOutOfEdges(sf::Vector2i head,
                                       sf::Vector2i stage_edge) const
{
    return (head.x < 0 or head.y < 0 or
            head.x >= stage_edge.x or head.y >= stage_edge.y);
}

double AutoController::distance_to_snake(Direction direction,
                                         sf::Vector2i head,
                                         std::vector<sf::Vector2i> body,
                                         sf::Vector2i stage_edge)
{
    double distance = 0.0;
    const auto containsPoint = [&head](const auto& part)
    { return head == part; };

    switch (direction)
    {
    case Direction::Up:
        --head.y;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
            --head.y;
            distance += 1.0;
        }
        LOG_DEBUG("Direction::Up to body element distance = ", distance);
        return (distance/static_cast<double>(stage_edge.y));
    case Direction::Up_Right:
        --head.y;
        ++head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
            --head.y;
            ++head.x;
            distance += std::sqrt(2);
        }
        LOG_DEBUG("Direction::Up_Right to body element distance = ", distance);
        return (distance/
                std::sqrt(static_cast<double>(stage_edge.y*stage_edge.y +
                                              stage_edge.x*stage_edge.x)));
    case Direction::Up_Left:
        --head.y;
        --head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
            --head.y;
            --head.x;
            distance += std::sqrt(2);
        }
        LOG_DEBUG("Direction::Up_Left to body element distance = ", distance);
        return (distance/
                std::sqrt(static_cast<double>(stage_edge.y*stage_edge.y +
                                              stage_edge.x*stage_edge.x)));
    case Direction::Down:
        ++head.y;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
           ++head.y;
           distance += 1.0;
        }
        LOG_DEBUG("Direction::Down to body element distance = ", distance);
        return (distance/static_cast<double>(stage_edge.y));
    case Direction::Down_Right:
        ++head.y;
        ++head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
            ++head.y;
            ++head.x;
            distance += std::sqrt(2);
        }
        LOG_DEBUG("Direction::Down_Right to body element distance = ", distance);
        return (distance/
                std::sqrt(static_cast<double>(stage_edge.y*stage_edge.y +
                                              stage_edge.x*stage_edge.x)));
    case Direction::Down_Left:
        ++head.y;
        --head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
            ++head.y;
            --head.x;
            distance += std::sqrt(2);
        }
        LOG_DEBUG("Direction::Down_Left to body element distance = ", distance);
        return (distance/
                std::sqrt(static_cast<double>(stage_edge.y*stage_edge.y +
                                              stage_edge.x*stage_edge.x)));
    case Direction::Left:
        --head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
               break;
            }
            --head.x;
            distance += 1.0;
        }
        LOG_DEBUG("Direction::Left to body element distance = ", distance);
        return (distance/static_cast<double>(stage_edge.x));
    case Direction::Right:
        ++head.x;
        while (not std::any_of(body.begin(), body.end(), containsPoint))
        {
            if (isSnakeOutOfEdges(head, stage_edge))
            {
               break;
            }
            ++head.x;
            distance += 1.0;
        }
        LOG_DEBUG("Direction::Right to body element distance = ", distance);
        return (distance/static_cast<double>(stage_edge.x));
    }
    return 0.0;
}


void AutoController::initialize()
{
    const auto FITNESS_FUNCTION = [&](const Member& member)
    { return fitness_function(member); };
    const auto FAIL_FUNCTION = [&](const Member& member)
    { return fail_function(member); };

    population_.create(POPULATION_SIZE, MUTATION_RATIO, INPUT_SIZE,
                       OUTPUT_SIZE, FITNESS_FUNCTION, FAIL_FUNCTION);
    LOG_INFO("Initialize new population size: [", POPULATION_SIZE,
             "] mutation chance: [", MUTATION_RATIO,
             "] input size: [", INPUT_SIZE,
             "] output size: [", OUTPUT_SIZE, "]");
}

/* 10% punkty za elementy węża wokół głowy
 * 35% odległość od jajka
 * 55% punkty za długość węża
 *
 * max 1pkt na 100ms
 *      [1][1][1][1][1]
 *      [1][2][2][2][1]
 *      [1][2][O][2][1]
 *      [1][2][X][2][1]
 *      [1][1][X][1][1]
 */
double AutoController::fitness_function(const Member&) const
{
    const auto TEN_PERCENT = 0.1;
    const auto THIRTY_FIVE_PERCENT = 0.40;
    const auto FIFTY_FIVE_PERCENT = 0.50;
    double f = TEN_PERCENT * elementsAroundHead() +
               THIRTY_FIVE_PERCENT * distanceFromFeedToHead() +
               FIFTY_FIVE_PERCENT * snakeLength();
    LOG_DEBUG("Returned fitness value [", f, "]");
    return f;
}

double AutoController::elementsAroundHead() const
{
    const auto head = stage_.getSnake().getHead();
    const auto body = stage_.getSnake().position();
    const auto start_p = sf::Vector2i(head.x - 2, head.y - 2);
    const auto end_p   = sf::Vector2i(head.x + 2, head.y + 2);
    const auto MAX_POINTS = 39.0;

    double points = 0.0;
    for (int i=start_p.x; i<=end_p.x; i++)
    {
        for (int j=start_p.y; j<=end_p.y; j++)
        {
            sf::Vector2i p(i, j);
            points += 1.0;
            if (i == (start_p.x + 1) or j == (start_p.y + 1) or
                i == (end_p.x - 1) or j == (end_p.y - 1))
            {
                points += 1.0;
            }
            else if (std::any_of(body.begin(), body.end(),
                                 [&p](const auto& x) { return p == x; }))
            {
                points -= 1.0;
            }
        }
    }
    LOG_DEBUG("Points for elements around head [", points, "] returns ",
              points/MAX_POINTS);
    return points/MAX_POINTS;
}

double AutoController::distanceFromFeedToHead() const
{
    const auto head = stage_.getSnake().getHead();
    const auto feed = stage_.getFeed().position();
    const auto MAX_DISTANCE = std::sqrt(static_cast<double>(
                                            (stage_.edge().x*stage_.edge().x) +
                                            (stage_.edge().y*stage_.edge().y)));
    const auto DISTANCE = std::sqrt(static_cast<double>(
                                        ((head.x-feed.x)*(head.x-feed.x)) +
                                        ((head.y-feed.y)*(head.y-feed.y))));
    LOG_DEBUG("Points for distance feed to head: ", DISTANCE/MAX_DISTANCE);
    return DISTANCE/MAX_DISTANCE;
}

double AutoController::snakeLength() const
{
    const auto START_SIZE = 4.0;
    const auto size = static_cast<double>(stage_.getSnake().position().size());
    LOG_DEBUG("Points for snake length: ", size-START_SIZE);
    return size-START_SIZE;
}

bool AutoController::fail_function(const Member& member) const
{
    if (gameTheEnd_)
    {
        LOG_INFO("Member failed. Fitness with precission ", member.fitness(),
                 " points");
        return true;
    }
    return false;
}


