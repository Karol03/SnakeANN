#pragma once
#include <vector>

#include "stage.h"
#include "utilities/config.hpp"
#include "NeuralNetwork/population.hpp"

class AutoController
{
public:
    AutoController(Stage& stage, bool& dead);
    int play();
    void initialize();

private:
    int playMemberFromFile(Member& member);
    int trainPlayers();

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

    bool isSnakeOutOfEdges(sf::Vector2i head, sf::Vector2i stage_edge) const;

    bool& gameTheEnd_;
    bool isMemberCanPlay_;
    Stage& stage_;
    unsigned current_id_;
    Population population_;

    /* POPULATION CONFIGURATION */
    const int POPULATION_SIZE = CONFIG::for_genetic_algorithm::POPULATION_SIZE;
    const double MUTATION_RATIO = CONFIG::for_genetic_algorithm::MUTATION_RATIO;
    const size_t INPUT_SIZE = CONFIG::for_genetic_algorithm::INPUT_SIZE;
    const size_t OUTPUT_SIZE = CONFIG::for_genetic_algorithm::OUTPUT_SIZE;
};
