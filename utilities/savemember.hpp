#pragma once
#include <memory>

#include "NeuralNetwork/neuralnetwork.hpp"
#include "NeuralNetwork/member.hpp"

class SaveMember
{
    struct Matrix
    {
        std::size_t input_size;
        std::size_t output_size;
        double* data;
    };

    SaveMember() = default;
public:
    SaveMember(std::unique_ptr<Member>& member);

    void save();
    void load(Member& member, const char* fileName);

    static bool isMemberCanPlay();
    static Member& getMemberFromFile();

private:
    void layer_to_matrix(const LayerDescription& layer, Matrix& raw);
    void data_to_layer(Neuron* raw, LayerDescription& layer, size_t size);
    bool is_file_exist(const char *fileName);

    void save_network(const NeuralNetwork& nn, const char* fileName);
    void load_network(Member& member, const char* fileName);

    static bool isMemberCanPlay_;
    static Member member_can_play;
    static unsigned file_number;
    std::unique_ptr<Member> member_;
};
