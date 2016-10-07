#ifndef BLOCK_HH
#define BLOCK_HH

#include <boost/cstdint.hpp>

struct Block
{
    static const boost::uint64_t DEFAULT_BLOCK_SIZE;
    static boost::uint64_t BLOCK_SIZE;

    const boost::uint64_t num_obs;
    boost::uint64_t num_vars_x;
    const double* vars_x_data;
    const boost::uint32_t* vars_x_names;
    boost::uint64_t num_vars_y;
    const double* vars_y_data;
    const boost::uint32_t* vars_y_names;
    double* cors;

    Block(const boost::uint64_t num_obs);
    ~Block();
    bool is_triangular() const;
};

inline Block::Block(const boost::uint64_t num_obs)
    : num_obs(num_obs),
    num_vars_x(0), vars_x_data(0), vars_x_names(0),
    num_vars_y(0), vars_y_data(0), vars_y_names(0),
    cors(new double[BLOCK_SIZE*BLOCK_SIZE])
{}

inline Block::~Block()
{
    delete[] cors;
}

inline bool Block::is_triangular() const
{
    return vars_x_data == vars_y_data;
}

#endif // BLOCK_HH
