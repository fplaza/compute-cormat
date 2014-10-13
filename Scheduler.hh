#ifndef SCHEDULER_HH
#define SCHEDULER_HH

#include "BlockingQueue.hh"
#include "Tile.hh"
#include "Matrix.hh"

class Scheduler
{
	private:
		const Matrix& mat_;
		BlockingQueue<Tile*>& tiles_to_recycle_;
		BlockingQueue<Tile*>& tiles_to_compute_;

	public:
		Scheduler(const Matrix& mat, BlockingQueue<Tile*>& tiles_to_recycle, BlockingQueue<Tile*>& tiles_to_compute_);
		~Scheduler();
		void run();
};

#endif // PRECOMPUTED_COR_MAT_READER_HH
