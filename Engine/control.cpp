#include "control.hpp"

Control::Control(Stage& stage)
    : State(State::TControlState, stage)
{}

void Control::execute()
{
    controller_.control(stage_);
    stage_.nextFrame();
}

State::Type Control::next()
{
    return TValidationState;
}

