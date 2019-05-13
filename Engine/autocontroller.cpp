#include <cmath>
#include <memory>

#include "autocontroller.hpp"
#include "utilities/filehandler.hpp"

#define UP_KEY    'w'
#define DOWN_KEY  's'
#define RIGHT_KEY 'd'
#define LEFT_KEY  'a'
#define NEW_GAME  'n'

namespace detail
{
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

AutoController::AutoController(Stage& stage)
    : stage_(stage)
    , current_id_(1)
{
}

void AutoController::checkIfFoundBestPlayer()
{
    const auto MEMBER_LOWER_BOUND = 500;
    std::unique_ptr<Member> bestMember;
    population_.getBestMember(bestMember);
    LOG_INFO("Best member fitness [", bestMember->fitness(), "/",
             MEMBER_LOWER_BOUND, "]");
}

int AutoController::play()
{
    const int NO_ONE_STILL_ALIVE = 0;
    if (population_.livingMembers() == NO_ONE_STILL_ALIVE)
    {
        LOG_INFO("All members are dead. Restarting and create new generation");
        checkIfFoundBestPlayer();
        const unsigned FIRST_MEMBER_ID = 1;
        population_.evolve();
        current_id_ = FIRST_MEMBER_ID;
        getchar();
        return NEW_GAME;
    }

    Member& member = population_.getMember(current_id_);
    if (member.isDead())
    {
        ++current_id_;
    }
    else
    {
        population_.one_member_prediction(member, getInputData());
        LOG_DEBUG("Best prediction member with id [", member.id, "]",
                  " is [", detail::chooseToPrint(member.prediction().choose),
                  "] with accuracy [", member.prediction().accuracy, "] ");
    }
    return detail::chooseToKey(member.prediction().choose);
}

std::vector<double> AutoController::getInputData()
{
    std::vector<double> input_data(24);
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
    input_data[6] = distance_to_snake(Direction::Up, head, body, edge);
    input_data[7] = distance_to_snake(Direction::Right, head, body, edge);
    input_data[8] = distance_to_snake(Direction::Down, head, body, edge);
    input_data[9] = distance_to_snake(Direction::Left, head, body, edge);
    return input_data;
}

double AutoController::angle_to_feed(sf::Vector2i head,
                                     sf::Vector2i feed)
{
    double x = static_cast<double>(feed.x - head.x);
    double y = static_cast<double>(feed.y - head.y);
    double fi = atan(y/x);
    LOG_DEBUG("Angle: fi = ", fi, "  result = ", fi / M_PI - 1);
    return fi / M_PI - 1;
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
    int distance = 0;
    switch (direction)
    {
    case Direction::Up:
        while (head.y-1 > 0)
        {
            --head.y;
            distance += 1;
        }
        LOG_DEBUG("Direction::Up distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.y)) -
                static_cast<double>(stage_edge.y)/2;
    case Direction::Down:
        while (head.y+1 < stage_edge.y)
        {
            ++head.y;
            distance += 1;
        }
        LOG_DEBUG("Direction::Down distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.y)) -
                static_cast<double>(stage_edge.y)/2;
    case Direction::Left:
        while (head.x-1 > 0)
        {
            --head.x;
            distance += 1;
        }
        LOG_DEBUG("Direction::Left distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.x)) -
                static_cast<double>(stage_edge.x)/2;
    case Direction::Right:
        while (head.x+1 < stage_edge.x)
        {
            ++head.y;
            distance += 1;
        }
        LOG_DEBUG("Direction::Right distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.x)) -
                static_cast<double>(stage_edge.x)/2;
    }
    return 0.0;
}

bool AutoController::isSnakeOutOfEdges(sf::Vector2i head,
                                       sf::Vector2i stage_edge)
{
    return (head.x < 0 or head.y < 0 or
            head.x >= stage_edge.x or head.y >= stage_edge.y);
}

double AutoController::distance_to_snake(Direction direction,
                                         sf::Vector2i head,
                                         std::vector<sf::Vector2i> body,
                                         sf::Vector2i stage_edge)
{
    int distance = 0;
    const auto containsPoint = [&head](const auto& part)
    { return head == part; };

    switch (direction)
    {
    case Direction::Up:
        do
        {
            --head.y;
            distance += 1;
            if (isSnakeOutOfEdges(head, stage_edge))
            {
                break;
            }
        }
        while (std::any_of(body.begin(), body.end(), containsPoint));
        LOG_DEBUG("Direction::Up to body element distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.y)) -
                static_cast<double>(stage_edge.y)/2;
    case Direction::Down:
        do
        {
           ++head.y;
           distance += 1;
           if (isSnakeOutOfEdges(head, stage_edge))
           {
               break;
           }
        }
        while (std::any_of(body.begin(), body.end(), containsPoint));
        LOG_DEBUG("Direction::Down to body element distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.y)) -
                static_cast<double>(stage_edge.y)/2;
    case Direction::Left:
        do
        {
            --head.x;
            distance += 1;
            if (isSnakeOutOfEdges(head, stage_edge))
            {
               break;
            }
        }
        while (std::any_of(body.begin(), body.end(), containsPoint));
        LOG_DEBUG("Direction::Left to body element distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.x)) -
                static_cast<double>(stage_edge.x)/2;
    case Direction::Right:
        do
        {
            ++head.x;
            distance += 1;
            if (isSnakeOutOfEdges(head, stage_edge))
            {
               break;
            }
        }
        while (std::any_of(body.begin(), body.end(), containsPoint));
        LOG_DEBUG("Direction::Right to body element distance = ", distance);
        return (static_cast<double>(distance)/
                 static_cast<double>(stage_edge.x)) -
                static_cast<double>(stage_edge.x)/2;
    }
    return 0.0;
}


void AutoController::initialize()
{
    const int POPULATION_SIZE = 100;
    const double MUTATION_RATIO = 0.015;
    const size_t INPUT_SIZE = 24;
    const size_t OUTPUT_SIZE = 4;
    const auto FITNESS_FUNCTION = [&](const Member& member)
    { return fitness_function(member); };
    const auto FAIL_FUNCTION = [&](const Member& member)
    { return fail_function(member); };

    population_.create(POPULATION_SIZE, MUTATION_RATIO, INPUT_SIZE,
                       OUTPUT_SIZE, FITNESS_FUNCTION, FAIL_FUNCTION);
    LOG_INFO("Initialize new population size: [", POPULATION_SIZE,
             "] mutation chance: [", MUTATION_RATIO,
             "] input size: [", INPUT_SIZE,
             "] output size: [", OUTPUT_SIZE);
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
    const auto THIRTY_FIVE_PERCENT = 0.35;
    const auto FIFTY_FIVE_PERCENT = 0.55;
    return TEN_PERCENT * elementsAroundHead() +
           THIRTY_FIVE_PERCENT * distanceFromFeedToHead() +
           FIFTY_FIVE_PERCENT * snakeLength();
}

double AutoController::elementsAroundHead() const
{
    const auto head = stage_.getSnake().getHead();
    const auto body = stage_.getSnake().position();
    const auto start_p = sf::Vector2i(head.x - 2, head.y - 2);
    const auto end_p   = sf::Vector2i(head.x + 2, head.y + 2);
    const auto MAX_POINTS = 30.0;

    double points = 0.0;
    for (int i=start_p.x; i<=end_p.x; i++)
    {
        for (int j=start_p.y; j<=end_p.y; i++)
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
    return points/MAX_POINTS;
}

double AutoController::distanceFromFeedToHead() const
{
    const auto head = stage_.getSnake().getHead();
    const auto feed = stage_.getFeed().position();
    const auto MAX_DISTANCE = (stage_.edge().x^2)+(stage_.edge().y^2);
    return (((head.x-feed.x)^2) + ((head.y-feed.y)^2))/MAX_DISTANCE;
}

double AutoController::snakeLength() const
{
    const auto body = stage_.getSnake().position();
    return static_cast<double>(body.size());
}

bool AutoController::fail_function(const Member& member) const
{
    const auto edge = stage_.edge();
    const auto body = stage_.getSnake().position();
    const auto head = stage_.getSnake().getHead();
    if (std::any_of(body.begin(), body.end(),
                    [&head](const auto& x) { return head == x; }) or
        (head.x < 0 or head.y < 0 or head.x >= edge.x or head.y >= edge.y))
    {
        LOG_INFO("Member failed. Fitness with precission", member.fitness(),
                 " points");
        return true;
    }
    return false;
}


