#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

class NeuralNetwork
{
public:
    using Neuron = double;
    using Neurons = std::vector<Neuron>;

    struct Output
    {
        void getBestPrediction(const Neurons& neurons)
        {
            all = neurons;
            double current_best = 0.0;
            for (size_t i=0; i<neurons.size(); i++)
            {
                if (current_best < neurons[i])
                {
                    current_best = neurons[i];
                    choose = static_cast<int>(i);
                }
            }
        }

        int choose;
        double accuracy;
        Neurons all;
    };

    enum class Layer
    {
        Input,
        Hidden,
        Output
    };

    enum class TFunction
    {
        None,
        Sigmoid
    };

private:
    struct Function
    {
    private:
        double sigmoid(const NeuralNetwork::Neuron& n)
        { return 1.0/(1.0 + std::exp(-n)); }

    public:
        Function(TFunction type) : type(type) {}
        double operator()(NeuralNetwork::Neuron neuron)
        {
            switch (type)
            {
            case TFunction::Sigmoid: return sigmoid(neuron);
            default: return neuron;
            }
        }
        const TFunction type;
    };

    struct LayerDescription
    {
        LayerDescription(std::size_t size)
            : size(size)
            , function(TFunction::None)
        {}

        LayerDescription(std::size_t size, NeuralNetwork::TFunction function)
            : size(size)
            , function(function)
        {}
        std::size_t size;
        NeuralNetwork::TFunction function;
    };

public:
    using Layers = std::vector<LayerDescription>;
    using ResultVector = std::vector<Function>;

    static void mutate(double& neuron);

    NeuralNetwork();

    NeuralNetwork& add(Layer layer, std::size_t layerSize);
    NeuralNetwork& create();
    NeuralNetwork& reset();
    NeuralNetwork& setFunction(std::size_t layer, TFunction function);
    NeuralNetwork& train(Neurons data_as_double);

    int layerSize(std::size_t i) const;
    size_t layersNumber() const { return layers_.size(); }
    Output prediction() const { return prediction_; }
    const std::vector<double>& neurons() const { return neurons_; }
    std::vector<double>& neurons() { return neurons_; }

private:
    template<typename ...Args>
    bool areConfigured(const Args&... ts)
    {
        std::vector<LayerDescription> v = {ts...};
        const auto isConfigured_ = [](const LayerDescription& layer)
            { return layer.size > 0; };
        return std::all_of(v.begin(), v.end(), isConfigured_);
    }

    Neurons multiplyAndTransform(const Neurons& matrix,
                                 const Neurons& vertex,
                                 size_t output_size,
                                 Function function);
    void addInputLayer(std::size_t size);
    void addHiddenLayer(std::size_t size);
    void addOutputLayer(std::size_t size);

    bool isLayersChanged;
    Output prediction_;
    Layers layers_;
    Neurons neurons_;
};
