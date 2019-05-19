#include "drawing.hpp"
#include "utilities/config.hpp"

Drawing::Drawing(Stage& stage)
    : State(State::TDrawingState, stage)
    , drawer_(isCloseWindowGame_)
{}

void Drawing::execute()
{
    if (CONFIG::for_window::DRAW_WINDOW)
    {
        drawer_.draw(stage_);
    }
}

State::Type Drawing::next()
{
    return TControlState;
}
