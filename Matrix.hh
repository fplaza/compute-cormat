#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <string>
#include <iostream>
#include <stdint.h>

class Matrix
{
	friend class BinaryMatrixReader;

	private:
		static const char FILE_TYPE[9];
		uint64_t num_vars_;
		uint64_t num_obs_;
		uint32_t* vars_ids_;
		double* vars_data_;
		Matrix(uint64_t num_vars, uint64_t num_obs);
		uint32_t& var_id(uint64_t v);
		double& data(uint64_t v, uint64_t o);

	public:
		~Matrix();
		uint64_t num_vars() const;
		uint64_t num_obs() const;
		uint32_t var_id(uint64_t v) const;
		const uint32_t* vars_ids() const;
		double data(uint64_t v, uint64_t o) const;
		const double* data() const;
};

std::ostream& operator<<(std::ostream& os, const Matrix& mat);

inline Matrix::Matrix(uint64_t num_vars, uint64_t num_obs)
	: num_vars_(num_vars), num_obs_(num_obs), vars_ids_(new uint32_t[num_vars]),  vars_data_(new double[num_vars*num_obs])
{};

inline Matrix::~Matrix()
{
	delete[] vars_ids_;
	delete[] vars_data_;
}

inline uint32_t& Matrix::var_id(uint64_t v)
{
	return vars_ids_[v];
}

inline uint32_t Matrix::var_id(uint64_t v) const
{
	return vars_ids_[v];
}

inline const uint32_t* Matrix::vars_ids() const
{
	return vars_ids_;
}

inline double& Matrix::data(uint64_t v, uint64_t o)
{
	return vars_data_[v*num_obs() + o];
}

inline double Matrix::data(uint64_t v, uint64_t o) const
{
	return vars_data_[v*num_obs() + o];
}

inline const double* Matrix::data() const
{
	return vars_data_;
}

inline uint64_t Matrix::num_vars() const
{
	return num_vars_;
}

inline uint64_t Matrix::num_obs() const
{
	return num_obs_;
}


#endif // MATRIX_HH
