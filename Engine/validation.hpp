#pragma once
#include "state.hpp"


class Validation : public State
{
public:
    Validation(Stage& stage);

    void execute() override;
    Type next() override;

private:
    bool isHeadOutsideStage(sf::Vector2i head, sf::Vector2i stage_edge);

};
