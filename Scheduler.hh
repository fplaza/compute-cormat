#ifndef SCHEDULER_HH
#define SCHEDULER_HH

#include "BlockingQueue.hh"
#include "Block.hh"
#include "Matrix.hh"

class Scheduler
{
    private:
        const Matrix& mat_;
        BlockingQueue<Block*>& blocks_to_recycle_;
        BlockingQueue<Block*>& blocks_to_compute_;

    public:
        Scheduler(const Matrix& mat, BlockingQueue<Block*>& blocks_to_recycle, BlockingQueue<Block*>& blocks_to_compute_);
        ~Scheduler();
        void run();
};

#endif // PRECOMPUTED_COR_MAT_READER_HH
