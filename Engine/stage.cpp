#include "stage.h"

Stage::Stage()
    : stageEdge_(STAGE_WIDTH, STAGE_HEIGHT), snake_( Snake() ), feed_( Feed( ))
{}

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
        getSnake().growUp();
        LOG_DEBUG("Snake eat feed and grown up");
        return;
    }
    getSnake().move();
}
