#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <string>
#include <boost/cstdint.hpp>

class Matrix
{
    friend class TextMatrixReader;
    friend class BinaryMatrixReader;
    friend class BinaryMatrixWriter;
    friend class MatrixManip;

    public:
        ~Matrix();
        boost::uint64_t num_vars() const;
        boost::uint64_t num_obs() const;
        const boost::uint32_t* vars_names() const;
        boost::uint32_t var_name(const boost::uint64_t var_id) const;
        double data(const boost::uint64_t var_id, const boost::uint64_t obs_id) const;
        const double* data() const;

    private:
        static const char FILE_TYPE[9];
        boost::uint64_t num_vars_;
        boost::uint64_t num_obs_;
        boost::uint32_t* const vars_names_;
        double* const  data_;
        Matrix(const boost::uint64_t num_vars, const boost::uint64_t num_obs);
        boost::uint32_t& var_name(const boost::uint64_t var_id);
        double& data(const boost::uint64_t var_id, const boost::uint64_t obs_id);
};

inline Matrix::~Matrix()
{
    delete[] data_;
}

inline boost::uint64_t Matrix::num_vars() const
{
    return num_vars_;
}

inline boost::uint64_t Matrix::num_obs() const
{
    return num_obs_;
}

inline const boost::uint32_t* Matrix::vars_names() const
{
    return vars_names_;
}

inline boost::uint32_t Matrix::var_name(const boost::uint64_t var_id) const
{
    return vars_names_[var_id];
}

inline double Matrix::data(const boost::uint64_t var_id, const boost::uint64_t obs_id) const
{
    return data_[var_id*num_obs() + obs_id];
}

    inline Matrix::Matrix(const boost::uint64_t num_vars, const boost::uint64_t num_obs)
: num_vars_(num_vars),
    num_obs_(num_obs),
    vars_names_(new boost::uint32_t[num_vars]),
    data_(new double[num_vars*num_obs])
{};

inline boost::uint32_t& Matrix::var_name(const boost::uint64_t var_id)
{
    return vars_names_[var_id];
}

inline double& Matrix::data(const boost::uint64_t var_id, const boost::uint64_t obs_id)
{
    return data_[var_id*num_obs() + obs_id];
}

inline const double* Matrix::data() const
{
    return data_;
}

#endif // MATRIX_HH
