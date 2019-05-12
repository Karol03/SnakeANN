#pragma once
#include "state.hpp"


class Drawing : public State
{
public:
    Drawing(Stage& stage);

    void execute() override;
    Type next() override;
};
