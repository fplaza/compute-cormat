#include "Scheduler.hh"

Scheduler::Scheduler(const Matrix& mat, BlockingQueue<Tile*>& tiles_to_recycle, BlockingQueue<Tile*>& tiles_to_compute)
	: mat_(mat), tiles_to_recycle_(tiles_to_recycle), tiles_to_compute_(tiles_to_compute)
{
	for (uint64_t i = 0; i < tiles_to_recycle_.max_size(); i++)
		tiles_to_recycle_.enqueue(new Tile(mat_.num_obs()));
};

Scheduler::~Scheduler()
{
	for (uint64_t i = 0; i < tiles_to_recycle_.max_size(); i++)
	{
		Tile* tile = tiles_to_recycle_.dequeue();
		delete tile;
	}
}

void Scheduler::run()
{
	for (uint64_t i = 0; i < mat_.num_vars(); i+= Tile::MAX_TILE_DIM)
	{
		for (uint64_t j = i; j < mat_.num_vars(); j+= Tile::MAX_TILE_DIM)
		{
			Tile* tile = tiles_to_recycle_.dequeue();
			tile->num_vars_x = std::min(Tile::MAX_TILE_DIM, mat_.num_vars()-i);
			tile->vars_x_ids = mat_.vars_ids() + i;
			tile->vars_x_data = mat_.data() + i*mat_.num_obs();
			tile->num_vars_y = std::min(Tile::MAX_TILE_DIM, mat_.num_vars()-j);
			tile->vars_y_ids = mat_.vars_ids() + j;
			tile->vars_y_data = mat_.data() + j*mat_.num_obs();

			tiles_to_compute_.enqueue(tile);
		}
	}

	tiles_to_compute_.enqueue(NULL);
}
