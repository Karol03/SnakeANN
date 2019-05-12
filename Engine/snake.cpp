#include "snake.h"

Snake::Snake() : Object (ObjectType::TSnake) {

}

Snake::~Snake() {

}

void Snake::setNewDirection(Direction newDirection) {
    this->direction_ = newDirection;
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
    head_ = position_.back();
}

const std::vector<sf::Vector2i>& Snake::position() {
    return position_;
}

void Snake::moveHead() {
    switch(direction_){
        case Direction::Down:
            position_.insert(position_.begin(), sf::Vector2i(head_.x, head_.y + 1)); //dodaje głowę na początek
            break;
        case Direction::Up:
            position_.insert(position_.begin(), sf::Vector2i(head_.x, head_.y - 1)); //dodaje głowę na początek
            break;
        case Direction::Left:
            position_.insert(position_.begin(), sf::Vector2i(head_.x - 1, head_.y)); //dodaje głowę na początek
            break;
        case Direction::Right:
            position_.insert(position_.begin(), sf::Vector2i(head_.x + 1, head_.y + 1)); //dodaje głowę na początek
            break;
    }
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
    return std::any_of(position_.begin(), position_.end(),
                       [this](sf::Vector2i tmp1) { return tmp1 == this->head_; });
}
