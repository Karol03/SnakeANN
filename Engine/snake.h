#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <algorithm>
#include "object.h"
#include "direction.h"
#include "utilities/logger.hpp"
#include <SFML/Graphics.hpp>

class Snake : private Object {
public:
    Snake();
    ~Snake();
    void setNewDirection(Direction newDirection);
    Direction getDirection() const;
    void move();
    void growUp();
    bool isItselfEat() const;
    const sf::Vector2i& getHead();
    const sf::Vector2i& getHead() const;
    const sf::Vector2i& getTail();
    const std::vector<sf::Vector2i>& position() const;
    sf::Vector2i nextHeadPosition() const;

private:
    void setHead();
    void moveHead();
    void moveTail();

    Direction direction_;
    sf::Vector2i head_;
    std::vector<sf::Vector2i> position_;
};

#endif // SNAKE_H
