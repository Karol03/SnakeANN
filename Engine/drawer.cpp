#include <SFML/Graphics.hpp>
#include "drawer.hpp"

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


Drawer::Drawer()
    : window_(sf::VideoMode(STAGE_WIDTH*TEXTURE_WIDTH, STAGE_HEIGHT*TEXTURE_HEIGHT), TITLE)
{
    loadTextures();
}

void Drawer::loadTextures()
{
    sf::Texture texture_head, texture_edge, texture_end,
            texture_tail_straight, texture_feed;
    if (not texture_head.loadFromFile(SNAKE_HEAD_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake head from \"./",
                  SNAKE_HEAD_TEXTURE, "\" failed");
        std::runtime_error("Error while loading textures");
    }
    textures_[Snake_head] = texture_head;

    if (not texture_edge.loadFromFile(SNAKE_TAIL_EDGE_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail edge from \"./",
                  SNAKE_TAIL_EDGE_TEXTURE, "\" failed");
        std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_edge] = texture_edge;

    if (not texture_end.loadFromFile(SNAKE_TAIL_END_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail end from \"./",
                  SNAKE_TAIL_END_TEXTURE, "\" failed");
        std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_end] = texture_end;

    if (not texture_tail_straight.loadFromFile(SNAKE_TAIL_STRAIGHT_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail straight part from \"./",
                  SNAKE_TAIL_STRAIGHT_TEXTURE, "\" failed");
        std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_straight] = texture_tail_straight;

    if (not texture_feed.loadFromFile(FEED_TEXTURE))
    {
        LOG_ERROR("Cannot load texture feed from \"./",
                  FEED_TEXTURE, "\" failed");
        std::runtime_error("Error while loading textures");
    }
    textures_[Feed_egg] = texture_feed;

}

void Drawer::draw(Stage& stage)
{
    if (window_.isOpen())
        {
            sf::Event event;
            while (window_.pollEvent(event)) // check events
            {
                LOG_INFO("Close window event handled");
                if (event.type == sf::Event::Closed)
                {
                    window_.close();
                }
            }
            window_.clear(sf::Color::Black);
            drawSnake(stage.getSnake());
            drawFeed(stage.getFeed());
            window_.display();
        }
}

void Drawer::drawSnake(const Snake& snake)
{
    const auto snake_body = snake.position();
    drawSnakeHead(snake);
    const auto FIRST_PART_AFTER_HEAD = 1;
    const auto LAST_PART_TAIL = 1;
    for (unsigned i=FIRST_PART_AFTER_HEAD;
         i<snake_body.size()-LAST_PART_TAIL; i++)
    {
        const auto part_type = getPart(snake_body[i-1], snake_body[i+1]);
        const auto direction = getDirection(snake_body[i-1], snake_body[i+1]);
    }
    drawSnakeTail(snake);
}

void Drawer::drawSnakeHead(const Snake& snake)
{
    sf::Sprite head(textures_[Snake_head]);
}

void Drawer::drawSnakeTail(const Snake& snake)
{

}

Drawer::Part_type Drawer::getPart(const Snake& snake)
{

}

void Drawer::drawFeed(const Feed& feed)
{
    const auto FEED_X_POSITION = feed.position().x * TEXTURE_WIDTH;
    const auto FEED_Y_POSITION = feed.position().y * TEXTURE_HEIGHT;
    sf::Sprite feed_sprite(textures_[Feed_egg]);
    feed_sprite.setPosition(FEED_X_POSITION, FEED_Y_POSITION);
    window_.draw(feed_sprite);
}
