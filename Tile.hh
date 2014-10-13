#ifndef TILE_HH
#define TILE_HH

#include <stdint.h>

struct Tile
{
	static const uint64_t MAX_TILE_DIM;

	const uint64_t num_obs;
	uint64_t num_vars_x;
	const double* vars_x_data;
	const uint32_t* vars_x_ids;
	uint64_t num_vars_y;
	const double* vars_y_data;
	const uint32_t* vars_y_ids;
	double* cors;

	Tile(uint64_t num_obs);
	~Tile();
	bool is_triangular() const;
};

inline Tile::Tile(uint64_t num_obs)
	: num_obs(num_obs),
	num_vars_x(0), vars_x_data(0), vars_x_ids(0),
	num_vars_y(0), vars_y_data(0), vars_y_ids(0),
	cors(new double[MAX_TILE_DIM*MAX_TILE_DIM])
{}

inline Tile::~Tile()
{
	delete[] cors;
}

inline bool Tile::is_triangular() const
{
	return vars_x_data == vars_y_data;
}

#endif // TILE_HH
