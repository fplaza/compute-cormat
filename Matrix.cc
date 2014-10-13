#include "Matrix.hh"

std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
	for (uint64_t curr_var = 0; curr_var < mat.num_vars(); curr_var++)
	{
		os << mat.var_id(curr_var);

		for (uint64_t curr_obs = 0; curr_obs < mat.num_obs(); curr_obs++)
			os << '\t' << mat.data(curr_var, curr_obs);

		os << '\n';
	}

	return os;
}

const char Matrix::FILE_TYPE[9] = "MAT_FILE";
