#include "control.hpp"
#include "drawing.hpp"
#include "validation.hpp"
#include "utilities/logger.hpp"

#include "game.hpp"

Game::Game()
    : currentState_(State::TControlState)
{
    states_[State::TControlState] = std::make_unique<Control>(stage);
    states_[State::TDrawingState] = std::make_unique<Drawing>(stage);
    states_[State::TValidationState] = std::make_unique<Validation>(stage);
}

int Game::run()
{
    do
    {
        currentState_ = states_[currentState_]->next();
        states_[currentState_]->execute();
    }
    while (not states_[currentState_]->isCloseWindowGame());
    LOG_INFO("Program finish work!");
    return 0;
}
