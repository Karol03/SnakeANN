#include "control.hpp"

Control::Control(Stage& stage)
    : State(State::TControlState, stage)
    , controller_(stage)
{}

void Control::execute()
{
    if (isFinished_)
    {
        if (controller_.isPlayerWantToNewGame())
        {
            stage_.reset();
            isFinished_ = false;
        }
        return;
    }
    controller_.control();
    stage_.nextFrame();
}

State::Type Control::next()
{
    return TValidationState;
}

