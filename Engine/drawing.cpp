#include "drawing.hpp"

Drawing::Drawing(Stage& stage)
    : State(State::TDrawingState, stage)
{}

void Drawing::execute()
{
    LOG_DEBUG();

}

State::Type Drawing::next()
{
    return TControlState;
}
