#include <fstream>
#include <fileapi.h>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "savemember.hpp"
#include "config.hpp"

bool SaveMember::isMemberCanPlay_ = false;
Member SaveMember::member_can_play(0);
unsigned SaveMember::file_number = 0;

namespace detail
{
using namespace std;
vector<wstring> get_all_files_names_within_folder(string folder)
{
    vector<wstring> names;
    string search_path_s = folder + "/member_*.bin";
    wstring search_path(search_path_s.begin(), search_path_s.end());
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}
}

bool SaveMember::isMemberCanPlay()
{
    auto member_named_files =
            detail::get_all_files_names_within_folder(CONFIG::for_main::DIRECTORY_WITH_GAME);
    if (not member_named_files.empty())
    {
        auto last_member_file = member_named_files.back();
        std::string member_file_name(last_member_file.begin(), last_member_file.end());
        SaveMember().load(member_can_play, member_file_name.c_str());
        isMemberCanPlay_ = true;
        LOG_INFO("Member loaded from file [", member_file_name, "]");
    }
    return isMemberCanPlay_;
}

Member& SaveMember::getMemberFromFile()
{
    if (not isMemberCanPlay_)
    {
        LOG_ERROR("There is no loaded member which can play");
        throw std::runtime_error("Invalid function call. No member to return");
    }
    return member_can_play;
}

SaveMember::SaveMember(std::unique_ptr<Member>& member)
{
    member_.swap(member);
}

void SaveMember::layer_to_matrix(const LayerDescription& layer,
                                 Matrix& raw)
{
    raw.input_size = layer.inputSize();
    raw.output_size = layer.outputSize();
    if (raw.data)
    {
        delete [] raw.data;
    }
    raw.data = new Neuron[raw.input_size * raw.output_size];
    for (size_t i=0; i<layer.getSize(); i++)
    {
        raw.data[i] = layer[i];
    }
}

void SaveMember::save_network(const NeuralNetwork& nn, const char* fileName)
{
    std::ofstream file(fileName, file.binary | file.out);
    if (not file.is_open())
    {
        LOG_ERROR("Failed to open ", fileName);
        return;
    }
    try
    {
        size_t ln_ = nn.layersNumber();
        file.write(reinterpret_cast<char*>(&ln_), sizeof(size_t));
        for (unsigned i=0; i<nn.layersNumber(); i++)
        {
            const auto& layer = nn.layer(i);
            Matrix matrix_to_save;
            layer_to_matrix(layer, matrix_to_save);
            int size = static_cast<int>(sizeof(Neuron)*layer.getSize());
            file.write(reinterpret_cast<char*>(&(matrix_to_save.input_size)),
                       sizeof(size_t));
            file.write(reinterpret_cast<char*>(&(matrix_to_save.output_size)),
                       sizeof(size_t));
            file.write(reinterpret_cast<char*>(&(matrix_to_save.data)),
                       size);
            delete [] matrix_to_save.data;
        }
        file.close();
    } catch (std::exception& e) {
        LOG_ERROR("Error until save neural network to file\n", e.what());
        file.close();
    }
    LOG_INFO("Neural network was saved to file ", fileName);
}

void SaveMember::save()
{
    if (not member_)
    {
        LOG_INFO("No member to save");
        return;
    }
    const auto& nn = member_->handler().neuralNetwork();
    std::string fileName("member_");
    fileName += std::to_string(file_number);
    fileName += ".bin";
    save_network(nn, fileName.c_str());
    ++file_number;
}

bool SaveMember::is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void SaveMember::load(Member& member, const char* fileName)
{
    if (not is_file_exist(fileName))
    {
        LOG_INFO("Cannot load file. File [./", fileName, "] no exist");
        return;
    }
    load_network(member, fileName);
}

void SaveMember::load_network(Member& member, const char* fileName)
{
    NeuralNetwork nn;
    NeuralNetworkHandler ann;
    std::ifstream file(fileName, file.binary | file.in);
    if (not file.is_open())
    {
        LOG_ERROR("Failed to open ", fileName);
        return;
    }
    try
    {
        size_t layers_number, input_size, output_size = 0;
        file.read(reinterpret_cast<char*>(&layers_number), sizeof(layers_number));
        for (size_t i=0; i<layers_number; i++)
        {
            file.read(reinterpret_cast<char*>(&input_size), sizeof(input_size));
            file.read(reinterpret_cast<char*>(&output_size), sizeof(output_size));
            TFunction f(TFunction::Sigmoid);
            if (i == 0)
            {
                member.input_size(input_size-1);
                f = TFunction::None;
            }
            size_t elements = input_size*output_size;
            int size = static_cast<int>(sizeof(Neuron)*elements);
            Neuron* data = new Neuron[elements];
            file.read(reinterpret_cast<char*>(data), size);

            LayerDescription layer(input_size, output_size, f);
            layer.create();
            data_to_layer(data, layer, input_size*output_size);
            delete [] data;
            nn.addLayer(layer);
        }
        member.output_size(output_size);
        file.close();
    } catch (std::exception& e) {
        LOG_ERROR("Load neural network to file fail\n", e.what());
        file.close();
        return;
    }
    ann.neuralNetwork(nn);
    member.setHandler(ann);
    LOG_INFO("Neural network loaded to member ", fileName);
}

void SaveMember::data_to_layer(Neuron* raw, LayerDescription& layer, size_t size)
{
    for (size_t i=0; i<size; i++)
    {
        layer[i] = raw[i];
    }
}
