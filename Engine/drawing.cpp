#include "drawing.hpp"

Drawing::Drawing(Stage& stage)
    : State(State::TDrawingState, stage)
{}

void Drawing::execute()
{
    drawer_.draw(stage_);
}

State::Type Drawing::next()
{
    return TControlState;
}
