#include "snake.h"
#include "utilities/generator.hpp"

Snake::Snake(int stage_width, int stage_height)
    : Object (ObjectType::TSnake)
    , direction_(Direction::Up)
{
    createBodyOnCenterStage(stage_width, stage_height);
}

Snake::~Snake() {

}

void Snake::createBodyOnCenterStage(int stage_width, int stage_height)
{
    auto point_first  = sf::Vector2i(stage_width/2, stage_height/2-1);
    auto point_second = sf::Vector2i(stage_width/2, stage_height/2);
    auto point_third  = sf::Vector2i(stage_width/2, stage_height/2+1);
    auto point_fourth = sf::Vector2i(stage_width/2, stage_height/2+2);
    position_.push_back(point_first);
    position_.push_back(point_second);
    position_.push_back(point_third);
    position_.push_back(point_fourth);
    setHead();
}

void Snake::setNewDirection(Direction newDirection) {
    this->direction_ = newDirection;
}

Direction Snake::getDirection() const
{
    return direction_;
}

const sf::Vector2i& Snake::getHead() {
    return head_;
}

const sf::Vector2i& Snake::getHead() const {
    return head_;
}

const sf::Vector2i& Snake::getTail() {
    return position_.back();
}

void Snake::setHead() {
    head_ = position_.front();
}

const std::vector<sf::Vector2i>& Snake::position() const {
    return position_;
}

sf::Vector2i Snake::nextHeadPosition() const {
    switch(direction_){
        case Direction::Down:
            return sf::Vector2i(head_.x, head_.y + 1);
        case Direction::Up:
            return sf::Vector2i(head_.x, head_.y - 1);
        case Direction::Left:
            return sf::Vector2i(head_.x - 1, head_.y);
        case Direction::Right:
            return sf::Vector2i(head_.x + 1, head_.y);
    }
    LOG_ERROR("Invalid head position");
    throw std::runtime_error("Invalid head position");
}

void Snake::moveHead() {
    auto head = nextHeadPosition();
    position_.insert(position_.begin(), head); //dodaje głowę na początek
}

void Snake::moveTail() {
    position_.erase(position_.end()); //usuwa ogonek, który jest na końcu
}

void Snake::move() {
    /**************************************************
     * Tutaj kwestia jak będziemy numerować mapę      *
     * Obecnie założyłem, że numerujemy od lewego,    *
     * górnego rogu jako punkt (0, 0)                 *
     **************************************************/
    moveHead();
    moveTail();
    setHead();
}

void Snake::growUp() {
    moveHead();
    setHead();
}

bool Snake::isItselfEat() const {
    return std::any_of(position_.begin()+1, position_.end(),
                       [this](sf::Vector2i tmp1) { return tmp1 == this->head_; });
}
