#include "feed.h"
#include "utilities/generator.hpp"
#include "utilities/logger.hpp"

Feed::Feed() : Object (ObjectType::TFeed) {
}

Feed::~Feed() {
}

bool Feed::compareVectors(const std::vector<sf::Vector2i> &vec1,
                          const sf::Vector2i &vec2) {
    return std::find(vec1.begin(), vec1.end(), vec2) != vec1.end();
}

void Feed::randomPosition(const std::vector<sf::Vector2i>& snake,
                          const int max_width,
                          const int max_height) {
    sf::Vector2i feedPlace;
    do{
        feedPlace.x = Generator::get_random_int(0, max_width);
        feedPlace.y = Generator::get_random_int(0, max_height);
        LOG_INFO("RANDOM [", feedPlace.x, " ", feedPlace.y, "]");
    } while(compareVectors(snake, feedPlace));

    this->position_ = feedPlace;
}

const sf::Vector2i& Feed::position() {
    return this->position_;
}

const sf::Vector2i& Feed::position() const {
    return this->position_;
}
