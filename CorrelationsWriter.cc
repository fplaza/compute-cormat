#include "CorrelationsWriter.hh"
#include <stdexcept>

void CorrelationsWriter::run()
{
    cors_file_ofs_.open(cors_file_.c_str(), cors_file_openmode_);

    Block* block;
    while ((block = blocks_to_write_.dequeue()) != NULL)
    {
        if (block->is_triangular())
            extract_cors_triangular_block(block);
        else
            extract_cors_rectangular_block(block);

        write_results();

        blocks_to_recycle_.enqueue(block);
    }

    cors_file_ofs_.close();
}

inline void CorrelationsWriter::extract_cors_rectangular_block(Block* block)
{
    num_cors_kept_ = 0;
    for (boost::uint64_t i = 0, offset= 0; i < block->num_vars_x; i++)
    {
        for (boost::uint64_t j = 0; j < block->num_vars_y; j++,offset++)
        {
            const float cor_val = static_cast<float>(block->cors[offset]);
            if (cor_val >= cor_threshold_)
            {
                cors_buffer_[num_cors_kept_].var1 = block->vars_x_names[i];
                cors_buffer_[num_cors_kept_].var2 = block->vars_y_names[j];
                cors_buffer_[num_cors_kept_].val = cor_val;
                num_cors_kept_++;
            }
        }
    }
}


inline void CorrelationsWriter::extract_cors_triangular_block(Block* block)
{
    num_cors_kept_ = 0;
    for (boost::uint64_t i = 0, offset = 1; i < block->num_vars_x; i++, offset+=i+1)
    {
        for (boost::uint64_t j = i+1; j < block->num_vars_y; j++,offset++)
        {
            const float cor_val = static_cast<float>(block->cors[offset]);
            if (cor_val >= cor_threshold_)
            {
                cors_buffer_[num_cors_kept_].var1 = block->vars_x_names[i];
                cors_buffer_[num_cors_kept_].var2 = block->vars_y_names[j];
                cors_buffer_[num_cors_kept_].val = cor_val;
                num_cors_kept_++;
            }
        }
    }
}

void TextCorrelationsWriter::write_results()
{
    for (boost::uint64_t i = 0; i < num_cors_kept_; i++)
    {
        cors_file_ofs_ << cors_buffer_[i].var1 << '\t' << cors_buffer_[i].var2  << '\t' << cors_buffer_[i].val << '\n';
    }
}

void BinaryCorrelationsWriter::write_results()
{
    cors_file_ofs_.write(reinterpret_cast<const char*>(cors_buffer_), num_cors_kept_*sizeof(Correlation));
}



const std::string CorrelationsWriterFactory::TEXT_FORMAT = "text";
const std::string CorrelationsWriterFactory::BINARY_FORMAT = "binary";

std::auto_ptr<CorrelationsWriter> CorrelationsWriterFactory::create_writer(const std::string& output_format, const std::string& cors_file, float cor_threshold,
        BlockingQueue<Block*>& blocks_to_write, BlockingQueue<Block*>& blocks_to_recycle)
{
    if (output_format == TEXT_FORMAT)
    {
        return std::auto_ptr<CorrelationsWriter>(new TextCorrelationsWriter(cors_file, cor_threshold, blocks_to_write, blocks_to_recycle));
    }
    else if (output_format == BINARY_FORMAT)
    {
        return std::auto_ptr<CorrelationsWriter>(new BinaryCorrelationsWriter(cors_file, cor_threshold, blocks_to_write, blocks_to_recycle));
    }
    else
    {
        throw (std::invalid_argument("error: " + output_format+  " is a unknown output format."));
    }
}

