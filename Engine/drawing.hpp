#pragma once
#include "drawer.hpp"
#include "state.hpp"


class Drawing : public State
{
public:
    Drawing(Stage& stage);

    void execute() override;
    Type next() override;

private:
    Drawer drawer_;
};
