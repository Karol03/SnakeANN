#include "feed.h"
#include "utilities/generator.hpp"

Feed::Feed() : Object (ObjectType::TFeed) {
}

Feed::~Feed() {
}

bool Feed::compareVectors(const std::vector<sf::Vector2i> &vec1, const sf::Vector2i &vec2) {
    for(const auto& position : vec1)
        if(position == vec2)
            return true;
    return false;
}

void Feed::randomPosition(const std::vector<sf::Vector2i> & snake) {
    sf::Vector2i feedPlace;
    do{
        feedPlace.x = static_cast<int>(Generator::get_random(0.0, 20.0));
        feedPlace.y = static_cast<int>(Generator::get_random(0.0, 20.0));
    } while(compareVectors(snake, feedPlace));

    this->position_ = feedPlace;
}

const sf::Vector2i& Feed::position() {
    return this->position_;
}

const sf::Vector2i& Feed::position() const {
    return this->position_;
}
