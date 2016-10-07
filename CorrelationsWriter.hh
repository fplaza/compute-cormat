#ifndef CORRELATIONS_WRITER_HH
#define CORRELATIONS_WRITER_HH

#include <string>
#include <fstream>
#include <memory>
#include "Block.hh"
#include "BlockingQueue.hh"

class CorrelationsWriter
{
    protected:
        struct Correlation
        {
            boost::uint32_t var1;
            boost::uint32_t var2;
            float val;
        };

        const std::string& cors_file_;
        std::ofstream cors_file_ofs_;
        std::ios_base::openmode cors_file_openmode_;
        const float cor_threshold_;
        BlockingQueue<Block*>& blocks_to_write_;
        BlockingQueue<Block*>& blocks_to_recycle_;
        Correlation* cors_buffer_;
        boost::uint64_t num_cors_kept_;

        void extract_cors_rectangular_block(Block* block);
        void extract_cors_triangular_block(Block* block);

    public:
        CorrelationsWriter(const std::string& cors_file, const std::ios_base::openmode cors_file_openmode, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle);
        void run();
        virtual void write_results() = 0;
};

inline CorrelationsWriter::CorrelationsWriter(const std::string& cors_file, const std::ios_base::openmode cors_file_openmode, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle)
    : cors_file_(cors_file),
    cors_file_ofs_(),
    cors_file_openmode_(cors_file_openmode),
    cor_threshold_(cor_threshold),
    blocks_to_write_(blocks_to_write),
    blocks_to_recycle_(blocks_to_recycle),
    cors_buffer_(new Correlation[Block::BLOCK_SIZE*Block::BLOCK_SIZE]),
    num_cors_kept_(0)
{}

class TextCorrelationsWriter : public CorrelationsWriter
{
    public:
        TextCorrelationsWriter(const std::string& cors_file, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle);
        virtual void write_results();

};


inline TextCorrelationsWriter::TextCorrelationsWriter(const std::string& cors_file, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle)
    : CorrelationsWriter(cors_file, std::ios_base::out | std::ios_base::trunc, cor_threshold, blocks_to_write, blocks_to_recycle)
{}

class BinaryCorrelationsWriter : public CorrelationsWriter
{
    public:
        BinaryCorrelationsWriter(const std::string& cors_file, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle);
        virtual void write_results();

};

inline BinaryCorrelationsWriter::BinaryCorrelationsWriter(const std::string& cors_file, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle)
    : CorrelationsWriter(cors_file, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary, cor_threshold, blocks_to_write, blocks_to_recycle)
{}

class CorrelationsWriterFactory
{
    public:
        static std::auto_ptr<CorrelationsWriter> create_writer(const std::string& output_format, const std::string& cors_file, float cor_threshold, BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle);
    private:
        static const std::string TEXT_FORMAT;
        static const std::string BINARY_FORMAT;
};

#endif // CORRELATIONS_WRITER_HH
