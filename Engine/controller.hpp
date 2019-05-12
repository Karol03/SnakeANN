#pragma once
#include "stage.h"

class Controller
{
public:
    Controller();

    void control(Stage& stage);

private:
    int opositKey(int c) const;
    int direction_to_key(Direction direction) const;
    bool isValid(int c) const;
    Direction key_to_direction(int key) const;

    int lastChar_;
};

