#include "stage.h"

Stage::Stage() : snake_( Snake() ), feed_( Feed( ))
{
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
    return (getSnake().getHead() == getFeed().position());
}

//void Stage::nextFrame() {
//    snake_.move();
//    isSnakeGrown_ = isSnakeGrown();
//    if(isSnakeGrown_)
//    {
//        if(getSnake().getTail() == getFeed().position())
//        {
//            getSnake().growUp();
//        }
//        else
//        {
//            getSnake().move();
//        }
//    }
//}
