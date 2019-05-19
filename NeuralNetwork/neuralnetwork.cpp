#include <utility>
#include <string>

#include "neuralnetwork.hpp"
#include "utilities/generator.hpp"

namespace detail
{
const char* print(TFunction func)
{
    switch (func)
    {
    case TFunction::None: return "TFunction::None";
    case TFunction::Sigmoid: return "TFunction::Sigmoid";
    }
    throw std::runtime_error("Invalid TFunction type");
}

std::string print(const Neurons& v)
{
    std::string str;
    for (auto d : v)
        str += std::to_string(d) + " ";
    return str;
}
}  // namespace detail

void NeuralNetwork::mutate(double& neuron)
{
    neuron = Generator::get_random();
}

NeuralNetwork::NeuralNetwork()
    : isLayersChanged(false)
    , allNetworkSize(0)
{}

LayerDescription& NeuralNetwork::add(size_t input_size, size_t output_size)
{
    const auto BIAS = 1;
    layers_.emplace_back(LayerDescription{input_size + BIAS, output_size});
    LOG_DEBUG("New layer with size of [", input_size," x ",
              output_size, "] weights");
    isLayersChanged = true;
    return layers_.back();
}

NeuralNetwork& NeuralNetwork::create()
{
    if (not isLayersChanged)
    {
        LOG_INFO("Nothing to create. None of layers changed");
        return *this;
    }
    isLayersChanged = false;
    allNetworkSize = 0;

    for (auto& layer : layers_)
    {
        layer.create();
        allNetworkSize += layer.getSize();
    }
    return *this;
}

NeuralNetwork& NeuralNetwork::reset()
{
    allNetworkSize = 0;
    layers_.clear();
    isLayersChanged = false;
    return *this;
}

NeuralNetwork& NeuralNetwork::train(Neurons data_as_double)
{
    for (const auto& layer : layers_)
    {
        data_as_double.push_back(1.0);
        data_as_double = layer.multiply_matrix_by_vector(data_as_double);
        for (auto& element : data_as_double)
        {
            layer.transform(element);
        }
    }
    prediction_.getBestPrediction(data_as_double);
    LOG_DEBUG("Neural network training finished prediction: [",
              prediction_.choose, "], accuracy [", prediction_.accuracy, "]");
    return *this;
}

int NeuralNetwork::layerSize(size_t i) const
{
    if (i > layers_.size())
    {
        LOG_ERROR("Neural network has no layer with index [", i, "]");
        return -1;
    }
    return static_cast<int>(layers_[i].getSize());
}

Neuron& NeuralNetwork::operator[](size_t i)
{
    if (layers_.empty())
    {
        LOG_ERROR("Layers are empty. Create first!");
        throw std::runtime_error("No layers cannot use operator[]");
    }
    if (i < layers_[0].getSize())
    {
        return layers_[0][i];
    }

    if (i < allNetworkSize)
    {
        size_t size_sum = 0;
        for (auto& layer : layers_)
        {
            if (i < (layer.getSize() + size_sum))
            {
                return layer[i - size_sum];
            }
            else
            {
                size_sum += layer.getSize();
            }
        }
    }
    LOG_ERROR("There is no ", i, " element in layers");
    throw std::runtime_error("Element index is too high!");
}

const LayerDescription& NeuralNetwork::layer(size_t i) const
{
    if (i >= layers_.size())
    {
        LOG_ERROR("There is no layer with index [", i, "]");
        throw std::out_of_range("Reference to layer with bad index");
    }
    return layers_[i];
}
