#ifndef CORRELATIONS_CALCULATOR_HH
#define CORRELATIONS_CALCULATOR_HH

#include "Tile.hh"
#include "BlockingQueue.hh"

class CorrelationsCalculator
{
	private:
		BlockingQueue<Tile*>& tiles_to_compute_;
		BlockingQueue<Tile*>& tiles_to_write_;
	public:
		CorrelationsCalculator(BlockingQueue<Tile*>& tiles_to_compute, BlockingQueue<Tile*>& tiles_to_write);
		void run();
};

inline CorrelationsCalculator::CorrelationsCalculator(BlockingQueue<Tile*>& tiles_to_compute, BlockingQueue<Tile*>& tiles_to_write)
	: tiles_to_compute_(tiles_to_compute), tiles_to_write_(tiles_to_write)
{};

#endif // CORRELATIONS_CALCULATOR_HH
