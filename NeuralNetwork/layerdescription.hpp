#pragma once

#include <cmath>
#include <vector>

using Neuron = double;
using Neurons = std::vector<Neuron>;

struct Output
{
    Output() : choose(0), accuracy(0.0) {}
    void getBestPrediction(const Neurons& neurons)
    {
        all = neurons;
        accuracy = 0.0;
        for (std::size_t i=0; i<neurons.size(); i++)
        {
            if (accuracy < neurons[i])
            {
                accuracy = neurons[i];
                choose = static_cast<int>(i);
            }
        }
    }

    int choose;
    double accuracy;
    Neurons all;
};

enum class TFunction
{
    None,
    Sigmoid
};

struct Function
{
private:
    double sigmoid(const Neuron& n)
    { return 1.0/(1.0 + std::exp(-n)); }

public:
    Function(TFunction type) : type(type) {}
    Neuron operator()(Neuron neuron)
    {
        switch(type)
        {
        case TFunction::None    : return neuron;
        case TFunction::Sigmoid : return sigmoid(neuron);
        }
        return neuron;
    }

    const TFunction type;
};

struct LayerDescription
{
    using Weights = std::vector<Neurons>;

    LayerDescription(std::size_t input_size,
                     std::size_t output_size,
                     TFunction function = TFunction::None);

    Weights multiply_layers(LayerDescription& layer) const
    {
        return multiply_matrix_by_matrix(layer.weights);
    }

    Neurons multiply_layer_by_vector(Neurons& layer_vector) const
    {
       return multiply_matrix_by_vector(layer_vector);
    }

    Neuron multiply_row_by_column(const Weights& w,
                                  size_t row,
                                  size_t column) const;
    Weights multiply_matrix_by_matrix(const Weights& w) const;
    Neurons multiply_matrix_by_vector(const Neurons& w) const;
    Neuron& operator[](size_t i);
    const Neuron& operator[](size_t i) const;
    Neuron& weight(size_t x, size_t y);
    LayerDescription& setFunction(TFunction f)
    {
        function = f;
        return *this;
    }

    Weights& swap(Weights& w)
    {
        std::swap(w, weights);
        return w;
    }

    bool hasWeights() const { return not weights.empty(); }
    void create() { fill_weights(); }
    void transform(Neuron& n) const { n = Function(function)(n); }
    std::size_t getSize() const { return input_size * output_size; }
    std::size_t inputSize() const { return input_size; }
    std::size_t outputSize() const { return output_size; }

private:
    void fill_weights();

    bool isCreated;
    std::size_t input_size;
    std::size_t output_size;
    TFunction function;
    Weights weights;
};
