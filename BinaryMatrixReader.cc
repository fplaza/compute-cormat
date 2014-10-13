#include "BinaryMatrixReader.hh"
#include <fstream>
#include <cstring>
#include <stdexcept>

Matrix BinaryMatrixReader::read(const std::string& matrix_path)
{
	std::ifstream ifs;
	ifs.open(matrix_path.c_str());

	char header[sizeof(Matrix::FILE_TYPE)-1];
	ifs.read(header, sizeof(header));

	if (std::memcmp(header, Matrix::FILE_TYPE, sizeof(header)) != 0)
		throw std::invalid_argument(matrix_path + " is not a valid binary matrix file.");

	uint64_t num_vars;
	uint64_t num_obs;

	ifs.read(reinterpret_cast<char*>(&num_vars), sizeof(uint64_t));
	ifs.read(reinterpret_cast<char*>(&num_obs), sizeof(uint64_t));

	Matrix mat(num_vars, num_obs);

	ifs.read(reinterpret_cast<char*>(mat.vars_ids_), sizeof(uint32_t)*mat.num_vars());
	ifs.read(reinterpret_cast<char*>(mat.vars_data_), sizeof(double)*mat.num_vars()*mat.num_obs());
	
	ifs.close();

	return mat;
}

