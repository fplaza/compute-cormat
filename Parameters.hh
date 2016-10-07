#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#include <string>

struct Parameters
{
    std::string precomputed_cormat_file;
    int num_threads;
    float cor_threshold;
    std::string output_format;
    std::string cors_file;

    static Parameters parse(int argc, char* argv[]);
    static void check_file_is_readable(const std::string& filepath);
    static void check_file_is_writable(const std::string& filepath);
    template <typename T>
    static void check_val_within_bounds(const std::string& val_name, T value, T lower, T higher);
};

#endif // PARAMETERS_HH
