#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "NeuralNetwork/layerdescription.hpp"
#include "utilities/logger.hpp"

class NeuralNetwork
{
public:
    using Layers = std::vector<LayerDescription>;
    using ResultVector = std::vector<Function>;

    static void mutate(double& neuron);

    NeuralNetwork();

    LayerDescription& add(std::size_t input_size, std::size_t output_size);
    NeuralNetwork& create();
    NeuralNetwork& reset();
    NeuralNetwork& train(Neurons data_as_double);

    int layerSize(std::size_t i) const;
    size_t layersNumber() const { return layers_.size(); }
    size_t size() const { return allNetworkSize; }
    Output prediction() const { return prediction_; }
    Neuron& operator[](size_t i);
    const LayerDescription& layer(size_t i) const;
    void addLayer(LayerDescription& layer) { layers_.push_back(layer); }

private:
    bool isLayersChanged;
    size_t allNetworkSize;
    Output prediction_;
    Layers layers_;
};
