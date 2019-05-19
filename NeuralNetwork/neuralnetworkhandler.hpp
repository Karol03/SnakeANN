#pragma once
#include <vector>

#include "neuralnetwork.hpp"

class NeuralNetworkHandler
{
public:
    enum class Type
    {
        Default,           // no hidden layer
        OneHiddenLayer,    // input layer - one hidden layer - output layer
        TwoHiddenLayers
    };

    NeuralNetworkHandler();

    void mixNN(NeuralNetworkHandler& ann_handler,
               double percentage_first_NN_part,
               double mutation_ratio);

    void create(std::size_t input_size, std::size_t output_size, Type type = Type::Default);
    void train(const Neurons& input_data);
    Output prediction() const;
    const NeuralNetwork& neuralNetwork() const { return nn_; }
    void neuralNetwork(NeuralNetwork& nn) { std::swap(nn_, nn); }

private:
    void mixAndMutate(NeuralNetwork& nn_second,
                      double percentage_first_NN_part,
                      double mutation_ratio);
    bool isLayersSizeEqual(const NeuralNetwork& nn1, const NeuralNetwork& nn2);

    NeuralNetwork nn_;
};
