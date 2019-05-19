#include "layerdescription.hpp"

#include "utilities/generator.hpp"
#include "utilities/logger.hpp"

LayerDescription::LayerDescription(std::size_t input_size,
                                   std::size_t output_size,
                                   TFunction function)
    : isCreated(false)
    , input_size(input_size)
    , output_size(output_size)
    , function(function)
{}

void LayerDescription::fill_weights()
{
    if (isCreated)
    {
        LOG_ERROR("Layer is already created");
        return;
    }
    Weights weights_(output_size);
    for (size_t i=0; i<output_size; i++)
    {
        Neurons vector_(input_size);
        Generator::generate_random_vector(vector_, input_size);
        std::swap(weights_[i], vector_);
    }
    std::swap(weights, weights_);
    LOG_DEBUG("Layer was filled by random weights");
    isCreated = true;
}

Neuron LayerDescription::multiply_row_by_column(
        const Weights& w, size_t row, size_t column) const
{
    if (not isCreated)
    {
        LOG_ERROR("Cannot multiply uninitialized weights");
        throw std::runtime_error("Uninitialized weights in layer");
    }
    if (weights[row].size() != w.size())
    {
        LOG_ERROR("Invalid dimensions, row size is [",
                  weights[row].size(),
                  "] column size is [", w.size(), "]");
        throw std::runtime_error("Invalid dimensions");
    }
    Neuron result = 0.0;
    for (size_t i = 0; i<weights[row].size(); i++)
    {
        result += weights[row][i] * w[i][column];
    }
    return result;
}

LayerDescription::Weights LayerDescription::multiply_matrix_by_matrix(
        const Weights& w) const
{
    if (not isCreated)
    {
        LOG_ERROR("Cannot multiply uninitialized weights");
        throw std::runtime_error("Uninitialized weights in layer");
    }
    if (weights[0].size() != w.size())
    {
        LOG_ERROR("Invalid dimensions, first matrix is [",
                  weights[0].size(),
                  " x ", weights.size(), "] second [",
                w[0].size(), " x ", w.size(),"]");
        throw std::runtime_error("Invalid dimensions");
    }

    Weights result(weights.size());
    const size_t row_size = w[0].size();
    for (size_t x = 0; x<weights.size(); x++)
    {
        Neurons new_row(row_size);
        for (size_t y = 0; y<row_size; y++)
        {
        new_row[y] =
                multiply_row_by_column(w, x, y);

        }
        std::swap(result[x], new_row);
    }
    return result;
}

Neurons LayerDescription::multiply_matrix_by_vector(const Neurons& w) const
{
    if (not isCreated)
    {
        LOG_ERROR("Cannot multiply uninitialized weights");
        throw std::runtime_error("Uninitialized weights in layer");
    }
    if (weights[0].size() != w.size())
    {
        LOG_ERROR("Invalid dimensions, matrix dimensions [",
                  weights[0].size(), " x ", weights.size(),
                  "] vector [", w.size(), "]");
        throw std::runtime_error("Invalid dimensions");
    }
    Neurons result(weights.size());
    size_t x = 0;
    for (const auto& row : weights)
    {
        auto sum = 0.0;
        for (size_t i = 0; i<row.size(); i++)
        {
            sum += row[i] * w[i];
        }
        result[x++] = sum;
    }
    return result;
}

Neuron& LayerDescription::operator[](size_t i)
{
    if (weights.empty())
    {
        LOG_ERROR("No weights in layer bad call!");
        throw std::runtime_error("Bad call to layer member");
    }
    if (i < weights[0].size())
    {
        return weights[0][i];
    }

    if (i < (weights.size()*weights[0].size()))
    {
        const size_t x = i%weights[0].size();
        const size_t y = i/weights[0].size();
        return weights[y][x];
    }
    LOG_ERROR("There is no ", i, " element in layer");
    throw std::runtime_error("Too small layer. Cannot find this weight");
}

const Neuron& LayerDescription::operator[](size_t i) const
{
    if (weights.empty())
    {
        LOG_ERROR("No weights in layer bad call!");
        throw std::runtime_error("Bad call to layer member");
    }
    if (i < weights[0].size())
    {
        return weights[0][i];
    }

    if (i < (weights.size()*weights[0].size()))
    {
        const size_t x = i%weights[0].size();
        const size_t y = i/weights[0].size();
        return weights[y][x];
    }
    LOG_ERROR("There is no ", i, " element in layer");
    throw std::runtime_error("Too small layer. Cannot find this weight");
}


Neuron& LayerDescription::weight(size_t x, size_t y)
{
    return weights[y][x];
}
