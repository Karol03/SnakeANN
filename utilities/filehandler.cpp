#include <fstream>

#include "filehandler.hpp"
#include "logger.hpp"

FileHandler::FileHandler(const char* fileName)
    : fileName_(fileName)
{}

void FileHandler::save(const std::vector<double>& save_from)
{
    try
    {
        size_t size = save_from.size();
        double* double_ptr = new double[size];
        std::ofstream file(fileName_, file.binary | file.out);
        if (not file.is_open())
        {
            LOG_ERROR("Failed to open ", fileName_);
            return;
        }
        vector_to_ptr(save_from, double_ptr);
        file.write(reinterpret_cast<char*>(double_ptr), size*sizeof(double));
        file.close();
        delete [] double_ptr;
    } catch (std::exception& e) {
        LOG_ERROR("Error until save neural network to file\n", e.what());

        throw e;
    }
    LOG_INFO("Neural network was saved to file ", fileName_);
}

void FileHandler::load(std::vector<double>& to_save, size_t size)
{
    try
    {
        double* double_ptr = new double[size];
        std::ifstream file(fileName_, file.binary | file.in);
        if (not file.is_open())
        {
            LOG_ERROR("Failed to open ", fileName_);
            return;
        }
        file.read(reinterpret_cast<char*>(double_ptr), size*sizeof(double));
        ptr_to_vector(double_ptr, to_save, size);
        file.close();
        delete [] double_ptr;
    } catch (std::exception& e) {
        LOG_ERROR("Error until save neural network to file\n", e.what());
        throw e;
    }
    LOG_INFO("Neural network was loaded from file ", fileName_);
}

void FileHandler::ptr_to_vector(double* ptr,
                                std::vector<double>& vec,
                                size_t size)
{
    std::vector<double> newVec(size);
    for (size_t i=0; i<size; i++)
    {
        newVec[i] = ptr[i];
    }
    std::swap(newVec, vec);
}

void FileHandler::vector_to_ptr(const std::vector<double>& vec, double* ptr)
{
    for (size_t i=0; i<vec.size(); i++)
    {
        ptr[i] = vec[i];
    }
}
