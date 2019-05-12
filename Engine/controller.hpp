#pragma once
#include "autocontroller.hpp"
#include "stage.h"

class Controller
{
public:
    Controller();

    void control(Stage& stage);
    bool isPlayerWantToNewGame();

private:
    int opositKey(int c) const;
    int direction_to_key(Direction direction) const;
    bool isValid(int c) const;
    Direction key_to_direction(int key) const;
    int autoControl(Stage& stage);
    int manualControl(Stage& stage);

    const bool isAutoPlay_;
    int lastChar_;
    AutoController autoController_;
};

