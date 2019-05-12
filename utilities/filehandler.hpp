#pragma once
#include <vector>

class FileHandler
{
public:
    FileHandler(const char* fileName);

    void save(const std::vector<double>& save_from);
    void load(std::vector<double>& to_save, std::size_t size);

private:
    void ptr_to_vector(double* ptr,
                       std::vector<double>& vec,
                       std::size_t size);
    void vector_to_ptr(const std::vector<double>& vec, double* ptr);

    const char* fileName_;
};
