#pragma once
#include <map>
#include "stage.h"

class Drawer
{
    enum Texture_type
    {
        Snake_head,
        Snake_tail_edge,
        Snake_tail_straight,
        Snake_tail_end,
        Feed_egg
    };

    enum Part_type
    {

    };

    enum Direction_type
    {
        UP_DOWN,
        UP_RIGHT,
        UP_LEFT,

        RIGHT_UP,
        RIGHT_LEFT,
        RIGHT_DOWN,

        DOWN_UP,
        DOWN_RIGHT,
        DOWN_LEFT,

        LEFT_UP,
        LEFT_RIGHT,
        LEFT_DOWN
    };

public:
    Drawer();

    void draw(Stage& stage);

private:
    void loadTextures();
    void drawSnake(const Snake& snake);
    void drawFeed(const Feed& feed);
    void drawSnakeHead(const Snake& snake);
    void drawSnakeTail(const Snake& snake);
    Part_type getPart(const Snake& snake);

    sf::RenderWindow window_;
    std::map<Texture_type, sf::Texture> textures_;
};
