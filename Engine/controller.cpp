#include <vector>
#include <conio.h>
#include <windows.h>

#include "controller.hpp"

#define UP_KEY    'w'
#define DOWN_KEY  's'
#define RIGHT_KEY 'd'
#define LEFT_KEY  'a'
#define NEW_GAME  'n'

Controller::Controller()
    : isAutoPlay_(false)
    , lastChar_(UP_KEY)
{}

void Controller::control(Stage& stage)
{
    Sleep(100);
    int c;
    if (isAutoPlay_)
    {
        c = autoControl(stage);
    }
    else
    {
        c = manualControl(stage);
    }
    lastChar_ = c;
    stage.getSnake().setNewDirection(key_to_direction(c));
}

bool Controller::isPlayerWantToNewGame()
{
    if (isAutoPlay_)
    {
        return true;
    }
    if (kbhit())
    {
        return getch() == NEW_GAME;
    }
    return false;
}

int Controller::autoControl(Stage& stage)
{
    int c = autoController_.play(stage);
    const auto currentDirection = stage.getSnake().getDirection();
    if (not isValid(c) or
            lastChar_ == c or
            (opositKey(c) == direction_to_key(currentDirection)))
    {
        return lastChar_;
    }
    return c;
}

int Controller::manualControl(Stage& stage)
{
    if (not kbhit())
    {
        return lastChar_;
    }
    int c = getch();
    const auto currentDirection = stage.getSnake().getDirection();
    if (not isValid(c) or
            lastChar_ == c or
            (opositKey(c) == direction_to_key(currentDirection)))
    {
        if (not isValid(c))
        {
            LOG_INFO("Invalid key [", c, "]");
        }
        return lastChar_;
    }
    if (c == NEW_GAME)
    {
        lastChar_ = UP_KEY;
        stage.reset();
        return lastChar_;
    }
    return c;
}

bool Controller::isValid(int c) const
{
    std::vector<int> chars{DOWN_KEY, UP_KEY, RIGHT_KEY, LEFT_KEY, NEW_GAME};
    return std::find(chars.begin(), chars.end(), c) != chars.end();
}

int Controller::opositKey(int c) const
{
    if (c == LEFT_KEY) return RIGHT_KEY;
    if (c == RIGHT_KEY) return LEFT_KEY;
    if (c == UP_KEY) return DOWN_KEY;
    if (c == DOWN_KEY) return UP_KEY;
    LOG_INFO("Cannot find oposite key for [", c, "]");
    return -1;
}

int Controller::direction_to_key(Direction direction) const
{
    switch (direction)
    {
    case Direction::Up: return UP_KEY;
    case Direction::Down: return DOWN_KEY;
    case Direction::Left: return LEFT_KEY;
    case Direction::Right: return RIGHT_KEY;
    }
    LOG_ERROR("Unsupported direction");
    throw std::runtime_error("Unsupported direction");
}

Direction Controller::key_to_direction(int key) const
{
    switch (key)
    {
    case UP_KEY: return Direction::Up;
    case DOWN_KEY: return Direction::Down;
    case LEFT_KEY: return Direction::Left;
    case RIGHT_KEY: return Direction::Right;
    }
    LOG_ERROR("Unsupported key [", key, "]");
    throw std::runtime_error("Unsupported key");
}
