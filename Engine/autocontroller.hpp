#pragma once
#include <vector>

#include "stage.h"
#include "NeuralNetwork/population.hpp"

class AutoController
{
public:
    AutoController(Stage& stage);
    int play();
    void initialize();

private:
    void checkIfFoundBestPlayer();
    std::vector<double> getInputData();
    double elementsAroundHead() const;
    double distanceFromFeedToHead() const;
    double snakeLength() const;
    double fitness_function(const Member& member) const;
    bool fail_function(const Member& member) const;

    double angle_to_feed(sf::Vector2i head, sf::Vector2i feed);
    double distance_to_feed();
    double distance_to_wall(Direction direction,
                            sf::Vector2i head,
                            sf::Vector2i stage_edge);
    double distance_to_snake(Direction direction,
                             sf::Vector2i head,
                             std::vector<sf::Vector2i> body,
                             sf::Vector2i stage_edge);

    bool isSnakeOutOfEdges(sf::Vector2i head, sf::Vector2i stage_edge);

    Stage& stage_;
    unsigned current_id_;
    Population population_;
};
