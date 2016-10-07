#ifndef CORRELATIONS_CALCULATOR_HH
#define CORRELATIONS_CALCULATOR_HH

#include "Block.hh"
#include "BlockingQueue.hh"

class CorrelationsCalculator
{
    private:
        const int num_threads_;
        BlockingQueue<Block*>& blocks_to_compute_;
        BlockingQueue<Block*>& blocks_to_write_;
    public:
        CorrelationsCalculator(const int num_threads, BlockingQueue<Block*>& blocks_to_compute, BlockingQueue<Block*>& blocks_to_write);
        void run();
};

inline CorrelationsCalculator::CorrelationsCalculator(const int num_threads, BlockingQueue<Block*>& blocks_to_compute, BlockingQueue<Block*>& blocks_to_write)
    : 
    num_threads_(num_threads),
    blocks_to_compute_(blocks_to_compute),
    blocks_to_write_(blocks_to_write)
{};

#endif // CORRELATIONS_CALCULATOR_HH
