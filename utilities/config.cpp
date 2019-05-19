#include "config.hpp"

const char* CONFIG::for_main::DIRECTORY_WITH_GAME =
        "C:/Users/karol/Documents/build-Snake-Desktop_Qt_5_11_1_MinGW_32bit-Debug";


std::size_t CONFIG::for_genetic_algorithm::FIRST_HIDDEN_LAYER_SIZE = 30;
std::size_t CONFIG::for_genetic_algorithm::SECOND_HIDDEN_LAYER_SIZE = 30;
int CONFIG::for_genetic_algorithm::POPULATION_SIZE = 3000;
double CONFIG::for_genetic_algorithm::MUTATION_RATIO = 0.02;
std::size_t CONFIG::for_genetic_algorithm::INPUT_SIZE = 18;
std::size_t CONFIG::for_genetic_algorithm::OUTPUT_SIZE = 4;

bool CONFIG::for_control::AUTO_PLAY_ON = true;
bool CONFIG::for_control::LOAD_MEMBERS_FROM_FILE = false;
bool CONFIG::for_control::SAVE_BEST_SNAKE_TO_FILE = false;
bool CONFIG::for_control::DELAY_AFTER_DRAWING = true;
unsigned long CONFIG::for_control::DELAY_TIME = 100;

bool CONFIG::for_window::DRAW_WINDOW = true;

double CONFIG::for_autocontroller_correction::LOWER_BOUND = 250;
double CONFIG::for_autocontroller_correction::UPPER_BOUND = 1000;
std::size_t CONFIG::for_autocontroller_correction::MINIMAL_LENGTH = 10;

