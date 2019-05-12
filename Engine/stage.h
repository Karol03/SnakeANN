#ifndef STAGE_H
#define STAGE_H

#include "snake.h"
#include "feed.h"
#include <SFML/Graphics.hpp>

class Stage
{
public:
    Stage();
    void nextFrame();
    bool isSnakeGrown() const;
    Feed& getFeed();
    const Feed& getFeed() const;
    Snake& getSnake();
    const Snake& getSnake() const;
    sf::Vector2i edge() const;

private:
    bool isSnakeGrown_;
    const sf::Vector2i stageEdge_;
    Snake snake_;
    Feed feed_;
};

#endif // STAGE_H
