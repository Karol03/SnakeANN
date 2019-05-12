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
private:
    bool isSnakeGrown_;
    sf::Vector2f stageEdge_;
    Snake snake_;
    Feed feed_;
};

#endif // STAGE_H
