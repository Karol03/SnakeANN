#include <utility>
#include <string>

#include "neuralnetwork.hpp"
#include "utilities/generator.hpp"
#include "utilities/logger.hpp"

namespace detail
{
const char* print(NeuralNetwork::TFunction func)
{
    switch (func)
    {
    case NeuralNetwork::TFunction::None: return "NeuralNetwork::TFunction::None";
    case NeuralNetwork::TFunction::Sigmoid: return "NeuralNetwork::TFunction::Sigmoid";
    }
    throw std::runtime_error("Invalid NeuralNetwork::TFunction type");
}

std::string print(const NeuralNetwork::Neurons& v)
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
{}

NeuralNetwork& NeuralNetwork::add(Layer layer, size_t layerSize)
{
    switch (layer)
    {
    case Layer::Input:
        addInputLayer(layerSize);
        break;
    case Layer::Hidden:
        addHiddenLayer(layerSize);
        break;
    case Layer::Output:
        addOutputLayer(layerSize);
        break;
    }
    LOG_DEBUG("New ", (layer == Layer::Input ? "[input]"
                                            : (layer == Layer::Hidden ? "[hidden]" : "[output]" ))
             ," with size of [", layerSize ,"] neurons");
    return *this;
}

void NeuralNetwork::addInputLayer(size_t size)
{
    isLayersChanged = true;
    layers_.insert(layers_.begin(), LayerDescription{size});
}

void NeuralNetwork::addHiddenLayer(size_t size)
{
    isLayersChanged = true;
    if (layers_.size() < 2)
    {
        layers_.emplace_back(LayerDescription{size});
        return;
    }
    layers_.insert(layers_.end() - 2, LayerDescription{size});
}

void NeuralNetwork::addOutputLayer(size_t size)
{
    isLayersChanged = true;
    layers_.emplace_back(LayerDescription{size});
}

NeuralNetwork& NeuralNetwork::create()
{
    if (not isLayersChanged)
    {
        LOG_INFO("Nothing to create. None of layers changed");
        return *this;
    }

    size_t neuronsNumber = 0;
    const auto BIAS = 1;
    for (size_t i=0; i<layers_.size()-1; i++)
    {
        auto& layer_One = layers_[i];
        auto& layer_Two = layers_[i + 1];
        if (not areConfigured(layer_One, layer_Two))
        {
            LOG_ERROR("Layers [", i, " or ", i+1, "] are not configured");
            return *this;
        }
        const auto between_layers_neurons = (layer_One.size + BIAS) * layer_Two.size;
        neuronsNumber += between_layers_neurons;
        LOG_DEBUG("Add [", between_layers_neurons, "] neurons between layer [",
                  i, " and ", i+1, "]");
    }
    if (neuronsNumber == 0)
    {
        LOG_ERROR("Cannot create. Too few layers");
        return *this;
    }
    Generator::generate_random_vector(neurons_, neuronsNumber);
    LOG_INFO("Generate [", neuronsNumber, "] neurons");
    return *this;
}

NeuralNetwork& NeuralNetwork::reset()
{
    layers_.clear();
    neurons_.clear();
    isLayersChanged = false;
    return *this;
}

NeuralNetwork& NeuralNetwork::setFunction(size_t layer, TFunction function)
{
    if (layer > layers_.size())
    {
        LOG_ERROR("Cannot set function for this layer. Layer ", layer, " not exist");
        return *this;
    }
    layers_[layer].function = std::move(function);
    LOG_DEBUG("Set layer [", layer, "] function to ", detail::print(function));
    return *this;
}

NeuralNetwork& NeuralNetwork::train(NeuralNetwork::Neurons data_as_double)
{
    Neurons::iterator layers_begin = neurons_.begin();
    Neurons::iterator layers_end;
    for (size_t i=0; i<layersNumber()-1; i++)
    {
        auto input_size = (layers_[i].size+1);
        auto output_size = (layers_[i+1].size);
        layers_end = layers_begin + static_cast<int>(input_size*output_size);
        Neurons current_layer(layers_begin, layers_end);
        Function function(layers_[i].function);
        data_as_double.push_back(1.0);
        auto multiply_result =
                multiplyAndTransform(
                    current_layer,
                    data_as_double,
                    output_size,
                    function);
        std::swap(data_as_double, multiply_result);
        layers_begin = layers_end;
    }
    prediction_.getBestPrediction(data_as_double);
    LOG_DEBUG("Neural network training finished");
    return *this;
}

NeuralNetwork::Neurons NeuralNetwork::multiplyAndTransform(
        const Neurons& matrix,
        const Neurons& vertex,
        size_t output_size,
        Function function)
{
    Neurons output(output_size);
    for (size_t i = 0; i<output_size; i++)
    {
        Neuron value = 0.0;
        for (size_t j = 0; j<vertex.size(); j++)
        {
            value += matrix[i*vertex.size()+j] * vertex[j];
        }
        output[i] = function(value);
    }
    LOG_DEBUG("Multiplication done, output size [", output_size, "]");
    return output;
}

int NeuralNetwork::layerSize(size_t i) const
{
    if (i > layers_.size())
    {
        LOG_ERROR("Neural network has no layer with index [", i, "]");
        return -1;
    }
    return static_cast<int>(layers_[i].size);
}
