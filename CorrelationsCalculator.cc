#include "CorrelationsCalculator.hh"
#include <omp.h>
#include <cblas.h>
#include <iostream>

extern "C" void openblas_set_num_threads(int num_threads);

void CorrelationsCalculator::run()
{
    Block* block;

    openblas_set_num_threads(num_threads_);
    while ((block = blocks_to_compute_.dequeue()) != NULL)
    {
        cblas_dgemm(CblasRowMajor,
                CblasNoTrans, CblasTrans,
                block->num_vars_x, block->num_vars_y, block->num_obs, 
                1.0, block->vars_x_data, block->num_obs, 
                block->vars_y_data, block->num_obs,
                0.0, block->cors, block->num_vars_x);

        blocks_to_write_.enqueue(block);
    }

    blocks_to_write_.enqueue(NULL);
}
