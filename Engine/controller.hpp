#pragma once
#include "autocontroller.hpp"
#include "stage.h"

class Controller
{
public:
    Controller(Stage& stage);

    void control();
    bool isPlayerWantToNewGame();

private:
    int opositKey(int c) const;
    int direction_to_key(Direction direction) const;
    bool isValid(int c) const;
    Direction key_to_direction(int key) const;
    int autoControl();
    int manualControl();

    const bool isAutoPlay_;
    bool isDead_;
    Stage& stage_;
    int lastChar_;
    AutoController autoController_;
};

