#pragma once

#include <vector>

/* For drawer */
#define TITLE "Snake"
#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

// head direction: UP
#define SNAKE_HEAD_TEXTURE "graphics/head.png"
// tail edge direction: UP <-> RIGHT
#define SNAKE_TAIL_EDGE_TEXTURE "graphics/tail_edge.png"
// tail end direction: UP
#define SNAKE_TAIL_END_TEXTURE "graphics/tail_end.png"
// tail straight direction: UP <-> DOWN
#define SNAKE_TAIL_STRAIGHT_TEXTURE "graphics/tail_straight.png"
#define FEED_TEXTURE "graphics/feed.png"

/* For stage */
#define STAGE_WIDTH  20
#define STAGE_HEIGHT 20

/* For control */
#define UP_KEY    'w'
#define DOWN_KEY  's'
#define RIGHT_KEY 'd'
#define LEFT_KEY  'a'
#define NEW_GAME  'n'

struct CONFIG
{
private:
    CONFIG() = default;

public:
    struct for_main
    {
        static const char* DIRECTORY_WITH_GAME;
    };

    struct for_genetic_algorithm
    {
        static std::size_t FIRST_HIDDEN_LAYER_SIZE;
        static std::size_t SECOND_HIDDEN_LAYER_SIZE;
        static int POPULATION_SIZE;
        static double MUTATION_RATIO;
        static std::size_t INPUT_SIZE;
        static std::size_t OUTPUT_SIZE;
    };

    struct for_control
    {
        static bool AUTO_PLAY_ON;
        static bool DELAY_AFTER_DRAWING;
        static bool LOAD_MEMBERS_FROM_FILE;
        static unsigned long DELAY_TIME;
        static bool SAVE_BEST_SNAKE_TO_FILE;
    };

    struct for_window
    {
        static bool DRAW_WINDOW;
    };

    struct for_autocontroller_correction
    {
        static double LOWER_BOUND;
        static double UPPER_BOUND;
        static std::size_t MINIMAL_LENGTH;
    };
};
