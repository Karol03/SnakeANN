#include <iostream>

#include "neuralnetworkhandler.hpp"
#include "utilities/generator.hpp"
#include "utilities/logger.hpp"

NeuralNetworkHandler::NeuralNetworkHandler()
{}

void NeuralNetworkHandler::mixNN(NeuralNetworkHandler& ann_handler,
                       double percentage_first_NN_part,
                       double mutation_ratio)
{
    if (not isLayersSizeEqual(nn_, ann_handler.nn_))
    {
        LOG_ERROR("Cannot mix layers, sizes are not equal");
        return;
    }
    mixAndMutate(ann_handler.nn_, percentage_first_NN_part, mutation_ratio);
}

void NeuralNetworkHandler::mixAndMutate(NeuralNetwork& nn_second,
                                 double percentage_first_NN_part,
                                 double mutation_ratio)
{
    auto& neurons = nn_.neurons();
    auto& secondNeurons = nn_second.neurons();
    const auto size = neurons.size();
    const auto howManyNeuronsSwap = percentage_first_NN_part*static_cast<double>(size);
    auto neuronInLayerIds = Generator::generate_int_vector(size);
    Generator::shuffle(neuronInLayerIds);
    const auto neuronInLayerIdsSize = neuronInLayerIds.size();

    for (unsigned j=0; j<static_cast<unsigned>(howManyNeuronsSwap); j++)
    {
        const unsigned idx = static_cast<unsigned>(neuronInLayerIds[j]);
        std::swap(neurons[idx], secondNeurons[idx]);
    }

    const auto howManyNeuronsMutate = mutation_ratio*static_cast<double>(size);
    for (unsigned j=0; j<static_cast<unsigned>(howManyNeuronsMutate); j++)
    {
        const unsigned idx_1 =
                static_cast<unsigned>(neuronInLayerIds[neuronInLayerIdsSize-2*j-1]);
        const unsigned idx_2 =
                static_cast<unsigned>(neuronInLayerIds[neuronInLayerIdsSize-2*j-2]);
        NeuralNetwork::mutate(neurons[idx_1]);
        NeuralNetwork::mutate(neurons[idx_2]);
    }
}

bool NeuralNetworkHandler::isLayersSizeEqual(const NeuralNetwork& nn1, const NeuralNetwork& nn2)
{
    if (nn1.layersNumber() != nn2.layersNumber())
    {
        LOG_ERROR("Different layers number");
        return false;
    }

    for (size_t i=0; i<nn1.layersNumber(); i++)
    {
        if (nn1.layerSize(i) != nn2.layerSize(i))
        {
            LOG_ERROR("Layer [", i,
                      (i == 1 ? "st" : (i == 2 ? "nd" : (i == 3 ? "rd" : (i == 0 ? "" : "th")))),
                      "] have different sizes [size in first nn: ", nn1.layerSize(i),
                      " | size in second nn: ", nn2.layerSize(i));
            return false;
        }
    }
    return true;
}

void NeuralNetworkHandler::create(std::size_t input_size, std::size_t output_size,
                                  Type type)
{
    size_t currentLayer = 0;
    const auto HIDDEN_LAYER_SIZE = 20;
    nn_.add(NeuralNetwork::Layer::Input, input_size);
       //.setFunction(currentLayer, NeuralNetwork::TFunction::Sigmoid);
    switch (type)
    {
    case Type::Default:
        break;
    case Type::OneHiddenLayer:
        nn_.add(NeuralNetwork::Layer::Hidden, HIDDEN_LAYER_SIZE)
           .setFunction(++currentLayer, NeuralNetwork::TFunction::Sigmoid);
        break;
    case Type::TwoHiddenLayers:
        nn_.add(NeuralNetwork::Layer::Hidden, HIDDEN_LAYER_SIZE)
           .setFunction(1, NeuralNetwork::TFunction::Sigmoid)
           .setFunction(2, NeuralNetwork::TFunction::Sigmoid);
        currentLayer += 2;
        break;
    }
    nn_.add(NeuralNetwork::Layer::Output, output_size)
       .setFunction(++currentLayer, NeuralNetwork::TFunction::Sigmoid)
       .create();
}

void NeuralNetworkHandler::train(const NeuralNetwork::Neurons& input_data)
{
    nn_.train(input_data);
}

NeuralNetwork::Output NeuralNetworkHandler::prediction() const
{
    return nn_.prediction();
}
