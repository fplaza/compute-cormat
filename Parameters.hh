#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#include <string>

struct Parameters
{
	std::string precomputed_cormat_file;
	std::string cors_file;
	std::string output_format;
	float cor_threshold;

	static Parameters parse(int argc, char* argv[]);
	static void check_file_is_readable(const std::string& filepath);
	static void check_file_is_writable(const std::string& filepath);
	template <typename T>
	static void check_val_within_bounds(const std::string& val_name, T value, T lower, T higher);
};

#endif // PARAMETERS_HH
