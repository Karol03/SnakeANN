#ifndef FEED_H
#define FEED_H

#include <vector>
#include "object.h"
#include <SFML/Graphics.hpp>

class Feed : private Object {
public:
    Feed();
    ~Feed();
    void randomPosition(const std::vector<sf::Vector2i> & snake);
    const sf::Vector2i& position();
    const sf::Vector2i& position() const;
private:
    bool compareVectors(const std::vector<sf::Vector2i> &vec1, const sf::Vector2i& vec2);

    sf::Vector2i position_;
};

#endif // FEED_H
