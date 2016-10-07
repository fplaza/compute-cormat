#include "BinaryMatrixReader.hh"
#include <fstream>
#include <cstring>
#include <stdexcept>

Matrix BinaryMatrixReader::read(const std::string& input_file)
{
    std::ifstream ifs;
    ifs.open(input_file.c_str());

    char header[sizeof(Matrix::FILE_TYPE)-1];
    ifs.read(header, sizeof(header));

    if (std::memcmp(header, Matrix::FILE_TYPE, sizeof(header)) != 0)
        throw std::invalid_argument(input_file + " is not a valid binary matrix file.");

    // Read dimensions
    boost::uint64_t num_vars;
    boost::uint64_t num_obs;
    ifs.read(reinterpret_cast<char*>(&num_vars), sizeof(boost::uint64_t));
    ifs.read(reinterpret_cast<char*>(&num_obs), sizeof(boost::uint64_t));

    // Allocate memory
    Matrix mat(num_vars, num_obs);

    // Read variables names
    ifs.read(reinterpret_cast<char*>(mat.vars_names_), sizeof(boost::uint32_t)*mat.num_vars());

    // Read matrix data
    ifs.read(reinterpret_cast<char*>(mat.data_), sizeof(double)*mat.num_vars()*mat.num_obs());

    ifs.close();

    return mat;
}

