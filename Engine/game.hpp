#pragma once
#include <map>
#include <memory>

#include "state.hpp"
#include "stage.h"

class Game
{
public:
    Game();

    int run();

private:
    State::Type currentState_;
    Stage stage;
    std::map<State::Type, std::unique_ptr<State>> states_;
};
