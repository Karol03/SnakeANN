#include <cmath>
#include <numeric>
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


namespace detail
{
const char* print(Drawer::Direction_type direction)
{
    switch (direction)
    {
    case Drawer::UP_DOWN: return "UP_DOWN";
    case Drawer::UP_RIGHT: return "UP_RIGHT";
    case Drawer::UP_LEFT: return "UP_LEFT";
    case Drawer::RIGHT_UP: return "RIGHT_UP";
    case Drawer::RIGHT_LEFT: return "RIGHT_LEFT";
    case Drawer::RIGHT_DOWN: return "RIGHT_DOWN";
    case Drawer::DOWN_UP: return "DOWN_UP";
    case Drawer::DOWN_RIGHT: return "DOWN_RIGHT";
    case Drawer::DOWN_LEFT: return "DOWN_LEFT";
    case Drawer::LEFT_UP: return "LEFT_UP";
    case Drawer::LEFT_RIGHT: return "LEFT_RIGHT";
    case Drawer::LEFT_DOWN: return "LEFT_DOWN";
    }
    LOG_ERROR("Unsupported direction [", direction, "]");
    throw std::runtime_error("Unsupported direction");
}

Direction oposite(Direction direction)
{
    switch (direction) {
    case Direction::Up: return Direction::Down;
    case Direction::Down: return Direction::Up;
    case Direction::Left: return Direction::Right;
    case Direction::Right: return Direction::Left;
    }
}

}  // namespace detail

Drawer::Drawer(bool& isCloseWindowGame)
    : isCloseWindowGame_(isCloseWindowGame)
    , window_(sf::VideoMode(STAGE_WIDTH*TEXTURE_WIDTH, STAGE_HEIGHT*TEXTURE_HEIGHT), TITLE)
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
        throw std::runtime_error("Error while loading textures");
    }
    textures_[Snake_head] = texture_head;

    if (not texture_edge.loadFromFile(SNAKE_TAIL_EDGE_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail edge from \"./",
                  SNAKE_TAIL_EDGE_TEXTURE, "\" failed");
        throw std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_edge] = texture_edge;

    if (not texture_end.loadFromFile(SNAKE_TAIL_END_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail end from \"./",
                  SNAKE_TAIL_END_TEXTURE, "\" failed");
        throw std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_end] = texture_end;

    if (not texture_tail_straight.loadFromFile(SNAKE_TAIL_STRAIGHT_TEXTURE))
    {
        LOG_ERROR("Cannot load texture snake tail straight part from \"./",
                  SNAKE_TAIL_STRAIGHT_TEXTURE, "\" failed");
        throw std::runtime_error("Error while loading textures");
    }
    textures_[Snake_tail_straight] = texture_tail_straight;

    if (not texture_feed.loadFromFile(FEED_TEXTURE))
    {
        LOG_ERROR("Cannot load texture feed from \"./",
                  FEED_TEXTURE, "\" failed");
        throw std::runtime_error("Error while loading textures");
    }
    textures_[Feed_egg] = texture_feed;
    LOG_INFO("All textures loaded");
}

void Drawer::draw(Stage& stage)
{
    if (window_.isOpen())
    {
        sf::Event event;
        while (window_.pollEvent(event)) // check events
        {
            if (event.type == sf::Event::Closed)
            {
                LOG_INFO("Close window event handled");
                isCloseWindowGame_ = true;
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
    const auto FIRST_PART_AFTER_HEAD = 1;
    const auto LAST_PART_TAIL = 1;

    drawSnakeHead(snake);
    Direction last_element_direction = detail::oposite(snake.getDirection());
    for (unsigned i=FIRST_PART_AFTER_HEAD;
         i<snake_body.size()-LAST_PART_TAIL; i++)
    {
        Direction_type direction;
        direction = getDirection(snake_body[i-1],
                snake_body[i+1],
                last_element_direction);
        drawTailElement(snake_body[i], direction);
    }
    drawSnakeTailEnd(snake);
}

void Drawer::drawSnakeHead(const Snake& snake)
{
    const float NINETY_DEGREE = 90.0;
    const auto HEAD_X_POSITION = snake.getHead().x * TEXTURE_WIDTH;
    const auto HEAD_Y_POSITION = snake.getHead().y * TEXTURE_HEIGHT;
    sf::Sprite head(textures_[Snake_head]);
    const auto direction = snake.getDirection();
    float angle = 0.0;
    switch (direction)
    {
    case Direction::Up:
        break;
    case Direction::Down:
        angle = 2*NINETY_DEGREE;
        break;
    case Direction::Left:
        angle = 3*NINETY_DEGREE;
        break;
    case Direction::Right:
        angle = NINETY_DEGREE;
        break;
    }
    head.setPosition(HEAD_X_POSITION, HEAD_Y_POSITION);
    rotate(head, angle);
    window_.draw(head);
}

void Drawer::drawSnakeTailEnd(const Snake& snake)
{
    const float NINETY_DEGREE = 90.0;
    const auto TAIL_X_POSITION = snake.position().back().x * TEXTURE_WIDTH;
    const auto TAIL_Y_POSITION = snake.position().back().y * TEXTURE_HEIGHT;
    const auto direction = getDirection(*(snake.position().end()-1),
                                        *(snake.position().end()-2));
    sf::Sprite tail(textures_[Snake_tail_end]);
    tail.setPosition(TAIL_X_POSITION, TAIL_Y_POSITION);
    float angle = 0.0;
    switch (direction)
    {
    case UP_DOWN:
        angle = 2*NINETY_DEGREE;
        break;
    case DOWN_UP:
        break;
    case LEFT_RIGHT:
        angle = NINETY_DEGREE;
        break;
    case RIGHT_LEFT:
        angle = 3*NINETY_DEGREE;
        break;
    default:
        LOG_ERROR("Cannot create element in this direction [",
                  detail::print(direction), "]");
        throw std::runtime_error("Cannot create tail in this point");
    }
    rotate(tail, angle);
    window_.draw(tail);
}

void Drawer::drawTailElement(const sf::Vector2i& element,
                             Direction_type direction)
{
    const float NINETY_DEGREE = 90.0;
    const auto TAIL_X_POSITION = element.x * TEXTURE_WIDTH;
    const auto TAIL_Y_POSITION = element.y * TEXTURE_HEIGHT;

    sf::Sprite body_element;
    body_element.setPosition(TAIL_X_POSITION, TAIL_Y_POSITION);
    float angle = 0.0;
    switch (direction)
    {
    case UP_DOWN:
    case DOWN_UP:
        body_element.setTexture(textures_[Snake_tail_straight]);
        break;
    case RIGHT_LEFT:
    case LEFT_RIGHT:
        body_element.setTexture(textures_[Snake_tail_straight]);
        angle = NINETY_DEGREE;
        break;
    case UP_RIGHT:
    case RIGHT_UP:
        body_element.setTexture(textures_[Snake_tail_edge]);
        break;
    case DOWN_RIGHT:
    case RIGHT_DOWN:
        angle = NINETY_DEGREE;
        body_element.setTexture(textures_[Snake_tail_edge]);
        break;
    case LEFT_DOWN:
    case DOWN_LEFT:
        body_element.setTexture(textures_[Snake_tail_edge]);
        angle = 2*NINETY_DEGREE;
        break;
    case UP_LEFT:
    case LEFT_UP:
        body_element.setTexture(textures_[Snake_tail_edge]);
        angle = 3*NINETY_DEGREE;
        break;
    }
    rotate(body_element, angle);
    window_.draw(body_element);
}

void Drawer::rotate(sf::Sprite& sprite, float angle)
{
    const auto pos = sprite.getPosition();
    if (std::fabs(angle-90.0f) < std::numeric_limits<float>::epsilon())
    {
        sprite.rotate(angle);
        sprite.setPosition(pos.x + TEXTURE_WIDTH, pos.y);
    }
    else if (std::fabs(angle-180.0f) < std::numeric_limits<float>::epsilon())
    {
        sprite.rotate(angle);
        sprite.setPosition(pos.x + TEXTURE_WIDTH, pos.y + TEXTURE_HEIGHT);
    }
    else if (std::fabs(angle-270.0f) < std::numeric_limits<float>::epsilon())
    {
        sprite.rotate(angle);
        sprite.setPosition(pos.x, pos.y + TEXTURE_HEIGHT);
    }

}

Drawer::Direction_type Drawer::getDirection(const sf::Vector2i& first,
                                            const sf::Vector2i& second)
{
    const auto delta_x = second.x - first.x;
    const auto delta_y = second.y - first.y;

    if (delta_x == 0 and delta_y > 0) return UP_DOWN;
    else if (delta_x == 0 and delta_y < 0) return DOWN_UP;
    else if (delta_x > 0 and delta_y == 0) return LEFT_RIGHT;
    else if (delta_x < 0 and delta_y == 0) return RIGHT_LEFT;
/*
    x   y  (from - to)
    1   1   R-U             // loop to right -->
   -1   1   U-L
   -1  -1   L-U
    1  -1   D-R

   -1  -1   U-L             // loop to left <--
   -1   1   R-D
    1  -1   L-U
    1   1   U-R

   -1  -1
    *//*
    else if (delta_x > 0 and delta_y > 0) return RIGHT_UP;
    else if (delta_x < 0 and delta_y < 0) return LEFT_UP;
    else if (delta_x > 0 and delta_y < 0) return DOWN_RIGHT;
    else if (delta_x < 0 and delta_y > 0) return UP_LEFT;*/
    LOG_ERROR("Unknown direction first_point: [", first.x, ", ", first.y,
              "] second_point: [", second.x, ", ", second.y, "]");
    throw std::runtime_error("Uknown direction");
}

Drawer::Direction_type Drawer::getDirection(
        const sf::Vector2i& first,
        const sf::Vector2i& second,
        Direction& l_e_d)
{
    const auto delta_x = second.x - first.x;
    const auto delta_y = second.y - first.y;

    if (delta_x == 0 and delta_y > 0) return UP_DOWN;
    else if (delta_x == 0 and delta_y < 0) return DOWN_UP;
    else if (delta_x > 0 and delta_y == 0) return LEFT_RIGHT;
    else if (delta_x < 0 and delta_y == 0) return RIGHT_LEFT;
/*
    x   y  (from - to)
    1   1   R-U             // loop to right -->
   -1   1   U-L
   -1  -1   L-U
    1  -1   D-R

   -1  -1   U-L             // loop to left <--
   -1   1   R-D
    1  -1   L-U
    1   1   U-R

   -1  -1
    */
    else if (delta_x > 0 and delta_y > 0 and l_e_d == Direction::Right)
    {
        l_e_d = Direction::Down;
        return LEFT_DOWN;
    }
    else if (delta_x > 0 and delta_y > 0 and l_e_d == Direction::Down)
    {
        l_e_d = Direction::Right;
        return UP_RIGHT;
    }
    else if (delta_x < 0 and delta_y < 0 and l_e_d == Direction::Left)
    {
        l_e_d = Direction::Up;
        return RIGHT_UP;
    }
    else if (delta_x < 0 and delta_y < 0 and l_e_d == Direction::Up)
    {
        l_e_d = Direction::Left;
        return DOWN_LEFT;
    }
    else if (delta_x < 0 and delta_y > 0 and l_e_d == Direction::Left)
    {
        l_e_d = Direction::Down;
        return RIGHT_DOWN;
    }
    else if (delta_x < 0 and delta_y > 0 and l_e_d == Direction::Down)
    {
        l_e_d = Direction::Left;
        return UP_LEFT;
    }
    else if (delta_x > 0 and delta_y < 0 and l_e_d == Direction::Right)
    {
        l_e_d = Direction::Up;
        return LEFT_UP;
    }
    else if (delta_x > 0 and delta_y < 0 and l_e_d == Direction::Up)
    {
        l_e_d = Direction::Right;
        return DOWN_RIGHT;
    }

    LOG_ERROR("Unknown direction first_point: [", first.x, ", ", first.y,
              "] second_point: [", second.x, ", ", second.y, "]");
    throw std::runtime_error("Uknown direction");

}

void Drawer::drawFeed(const Feed& feed)
{
    const auto FEED_X_POSITION = feed.position().x * TEXTURE_WIDTH;
    const auto FEED_Y_POSITION = feed.position().y * TEXTURE_HEIGHT;
    sf::Sprite feed_sprite(textures_[Feed_egg]);
    feed_sprite.setPosition(FEED_X_POSITION, FEED_Y_POSITION);
    window_.draw(feed_sprite);
}
