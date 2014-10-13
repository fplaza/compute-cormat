#include "CorrelationsCalculator.hh"
#include <mkl.h>

void CorrelationsCalculator::run()
{
	Tile* tile;
	while ((tile = tiles_to_compute_.dequeue()) != NULL)
	{
		cblas_dgemm(CblasRowMajor,
				CblasNoTrans, CblasTrans,
				tile->num_vars_x, tile->num_vars_y, tile->num_obs, 
				1.0, tile->vars_x_data, tile->num_obs, 
				tile->vars_y_data, tile->num_obs,
				0.0, tile->cors, tile->num_vars_x);
		
		tiles_to_write_.enqueue(tile);
	}

	tiles_to_write_.enqueue(NULL);
}
