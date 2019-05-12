#pragma once

#include "stage.h"
#include "NeuralNetwork/population.hpp"

class AutoController
{
public:
    AutoController();
    int play(Stage&)
    {
    return 0;
    }

private:
    Population population;
};
