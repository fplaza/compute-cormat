#include <iostream>
#include <new>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "Parameters.hh"
#include "TimeProfiler.hh"
#include "BlockingQueue.hh"
#include "BinaryMatrixReader.hh"
#include "Scheduler.hh"
#include "CorrelationsCalculator.hh"
#include "CorrelationsWriter.hh"

void no_more_memory()
{
    std::cerr << "error: memory allocation failed." << std::endl;
    std::exit (1);
}

int main(int argc, char *argv[])
{
    std::set_new_handler(no_more_memory);

    try
    {
        const Parameters& pars = Parameters::parse(argc, argv);

        TimeProfiler time_profiler;
        time_profiler.start_new_timer("Total");

        std::cout << "Reading precomputed correlation matrix..." << std::endl;
        time_profiler.start_new_timer("Reading matrix");
        const Matrix& mat= BinaryMatrixReader::read(pars.precomputed_cormat_file);
        time_profiler.stop_last_timer();
        std::cout << "Done. Matrix has " << mat.num_vars() << " variables made up of " << mat.num_obs() << " observations.\n" << std::endl;

        BlockingQueue<Block*> blocks_to_recycle(2);
        BlockingQueue<Block*> blocks_to_compute(2);
        BlockingQueue<Block*> blocks_to_write(2);

        Scheduler scheduler(mat, blocks_to_recycle, blocks_to_compute);
        CorrelationsCalculator calculator(pars.num_threads, blocks_to_compute, blocks_to_write);
        std::auto_ptr<CorrelationsWriter>  writer =
            CorrelationsWriterFactory::create_writer(pars.output_format, pars.cors_file, pars.cor_threshold, blocks_to_write, blocks_to_recycle);

        std::cout << "Computing correlations..." << std::endl;
        time_profiler.start_new_timer("Computing correlations");
        boost::thread scheduler_thread(boost::bind(&Scheduler::run, &scheduler));
        boost::thread calculator_thread(boost::bind(&CorrelationsCalculator::run, &calculator));
        boost::thread writer_thread(boost::bind(&CorrelationsWriter::run, writer.get()));

        scheduler_thread.join();
        calculator_thread.join();
        writer_thread.join();
        time_profiler.stop_last_timer();
        std::cout << "Done\n" << std::endl;

        time_profiler.stop_last_timer();
        std::cout << time_profiler;

    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
