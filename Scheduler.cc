#include "Scheduler.hh"

Scheduler::Scheduler(const Matrix& mat, BlockingQueue<Block*>& blocks_to_recycle, BlockingQueue<Block*>& blocks_to_compute)
    : mat_(mat),
    blocks_to_recycle_(blocks_to_recycle),
    blocks_to_compute_(blocks_to_compute)
{
    for (boost::uint64_t i = 0; i < blocks_to_recycle_.max_size(); i++)
        blocks_to_recycle_.enqueue(new Block(mat_.num_obs()));
};

Scheduler::~Scheduler()
{
    for (boost::uint64_t i = 0; i < blocks_to_recycle_.max_size(); i++)
    {
        Block* block = blocks_to_recycle_.dequeue();
        delete block;
    }
}

void Scheduler::run()
{
    for (boost::uint64_t i = 0; i < mat_.num_vars(); i+= Block::BLOCK_SIZE)
    {
        for (boost::uint64_t j = i; j < mat_.num_vars(); j+= Block::BLOCK_SIZE)
        {
            Block* block = blocks_to_recycle_.dequeue();
            block->num_vars_x = std::min(Block::BLOCK_SIZE, mat_.num_vars()-i);
            block->vars_x_names = mat_.vars_names() + i;
            block->vars_x_data = mat_.data() + i*mat_.num_obs();
            block->num_vars_y = std::min(Block::BLOCK_SIZE, mat_.num_vars()-j);
            block->vars_y_names = mat_.vars_names() + j;
            block->vars_y_data = mat_.data() + j*mat_.num_obs();

            blocks_to_compute_.enqueue(block);
        }
    }

    blocks_to_compute_.enqueue(NULL);
}
