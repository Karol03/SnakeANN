#pragma once
#include <map>
#include "stage.h"

class Drawer
{
    enum Texture_type
    {
        Snake_head = 0,
        Snake_tail_edge,
        Snake_tail_straight,
        Snake_tail_end,
        Feed_egg
    };
public:
    enum Direction_type
    {
        UP_DOWN = 0,
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
    Drawer();

    void draw(Stage& stage);

private:
    void loadTextures();
    void drawSnake(const Snake& snake);
    void drawFeed(const Feed& feed);
    void drawSnakeHead(const Snake& snake);
    void drawSnakeTailEnd(const Snake& snake);
    void drawTailElement(const sf::Vector2i& element,
                         Direction_type direction);
    void rotate(sf::Sprite& sprite, float angle);
    Direction_type getDirection(const sf::Vector2i& first,
                                const sf::Vector2i& second);
    Direction_type getDirection(const sf::Vector2i& first,
                                const sf::Vector2i& second,
                                Direction& last_direction);

    sf::RenderWindow window_;
    std::map<Texture_type, sf::Texture> textures_;
};
