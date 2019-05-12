#include "stage.h"

Stage::Stage()
    : stageEdge_(STAGE_WIDTH, STAGE_HEIGHT), snake_(STAGE_WIDTH, STAGE_HEIGHT), feed_( Feed( ))
{
    feed_.randomPosition(snake_.position(), STAGE_WIDTH, STAGE_HEIGHT);
}

void Stage::reset()
{
    Snake newSnake(STAGE_WIDTH, STAGE_HEIGHT);
    Feed newFeed;
    std::swap(snake_, newSnake);
    std::swap(feed_, newFeed);
    feed_.randomPosition(snake_.position(), STAGE_WIDTH, STAGE_HEIGHT);
}

sf::Vector2i Stage::edge() const
{
    return stageEdge_;
}

Snake& Stage::getSnake() {
    return this->snake_;
}

const Snake& Stage::getSnake() const {
    return this->snake_;
}

Feed& Stage::getFeed() {
    return this->feed_;
}

const Feed& Stage::getFeed() const {
    return this->feed_;
}

bool Stage::isSnakeGrown() const {
    return (getSnake().nextHeadPosition() == getFeed().position());
}

void Stage::nextFrame() {
    if(isSnakeGrown())
    {
        LOG_DEBUG("Snake eat feed and grown up");
        snake_.growUp();
        feed_.randomPosition(snake_.position(), STAGE_WIDTH, STAGE_HEIGHT);
        return;
    }
    snake_.move();
}
