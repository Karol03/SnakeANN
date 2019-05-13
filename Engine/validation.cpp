#include "validation.hpp"

Validation::Validation(Stage& stage)
    : State(State::TValidationState, stage)
{}

void Validation::execute()
{
    if (isFinished_)
    {
        return;
    }
    const Snake& snake = stage_.getSnake();
    const auto snakeEatItself = snake.isItselfEat();
    const auto snakeLeftStage = isHeadOutsideStage(snake.getHead(), stage_.edge());
    if (snakeEatItself or snakeLeftStage)
    {
        LOG_INFO("Game end, ",
                 (snakeEatItself ? "=== snake ate himself ===" : ""),
                 (snakeLeftStage ? "=== snake left the stage ===" : ""));
        LOG_INFO("You got: [", snake.position().size(), "] points!");
        isFinished_ = true;
    }
}

bool Validation::isHeadOutsideStage(sf::Vector2i head, sf::Vector2i stage_edge)
{
    if (head.x < 0 or head.y < 0 or head.x >= stage_edge.x or head.y >= stage_edge.y)
    {
        LOG_INFO("Snake is outside stage head:[", head.x, ", ", head.y,
                 "] stage_edge:[", stage_edge.x, ", ", stage_edge.y, "]");
        return true;
    }
    return false;
}

State::Type Validation::next()
{
    return TDrawingState;
}
