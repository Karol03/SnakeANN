#pragma once
#include "controller.hpp"
#include "state.hpp"

class Control : public State
{
public:
    Control(Stage& stage);

    void execute() override;
    Type next() override;

private:
    Controller controller_;
};
