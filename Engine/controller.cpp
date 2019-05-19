#include <vector>
#include <conio.h>
#include <windows.h>

#include "controller.hpp"
#include "utilities/config.hpp"

Controller::Controller(Stage& stage)
    : isAutoPlay_(CONFIG::for_control::AUTO_PLAY_ON)
    , isDead_(false)
    , stage_(stage)
    , lastChar_(UP_KEY)
    , autoController_(stage, isDead_)
{
    if (isAutoPlay_)
    {
        LOG_INFO("Auto control turn on, initialize auto_controller");
        autoController_.initialize();
    }
}

void Controller::control()
{
    if (CONFIG::for_control::DELAY_AFTER_DRAWING)
    {
        Sleep(CONFIG::for_control::DELAY_TIME);
    }

    int c;
    if (isAutoPlay_)
    {
        c = autoControl();
        if (c == NEW_GAME)
        {
            lastChar_ = UP_KEY;
            stage_.reset();
            return;
        }
    }
    else
    {
        c = manualControl();
    }
    lastChar_ = c;
    stage_.getSnake().setNewDirection(key_to_direction(c));
}

bool Controller::isPlayerWantToNewGame()
{
    isDead_ = true;
    if (isAutoPlay_)
    {
        if (autoControl() == NEW_GAME)
        {
            isDead_ = false;
            lastChar_ = UP_KEY;
            return true;
        }
    }
    if (kbhit())
    {
        if (getch() == NEW_GAME)
        {
            isDead_ = false;
            lastChar_ = UP_KEY;
            return true;
        }
    }
    return false;
}

int Controller::autoControl()
{
    int c = autoController_.play();
    if (c == NEW_GAME)
    {
        lastChar_ = UP_KEY;
        stage_.reset();
        return NEW_GAME;
    }

    const auto currentDirection = stage_.getSnake().getDirection();
    if (not isValid(c) or
            lastChar_ == c or
            (opositKey(c) == direction_to_key(currentDirection)))
    {
        LOG_DEBUG("Last char restored");
        return lastChar_;
    }
    return c;
}

int Controller::manualControl()
{
    if (not kbhit())
    {
        return lastChar_;
    }
    int c = getch();
    const auto currentDirection = stage_.getSnake().getDirection();
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
        stage_.reset();
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
    default: break;
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
