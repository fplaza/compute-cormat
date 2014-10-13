#include "CorrelationsWriter.hh"
#include <stdexcept>

void CorrelationsWriter::run()
{
	cors_file_ofs_.open(cors_file_.c_str(), cors_file_openmode_);

	Tile* tile;
	while ((tile = tiles_to_write_.dequeue()) != NULL)
	{
		if (tile->is_triangular())
			extract_cors_triangular_tile(tile);
		else
			extract_cors_rectangular_tile(tile);

		write_results();

		tiles_to_recycle_.enqueue(tile);
	}

	cors_file_ofs_.close();
}

inline void CorrelationsWriter::extract_cors_rectangular_tile(Tile* tile)
{
	num_cors_kept_ = 0;
	for (uint64_t i = 0, offset= 0; i < tile->num_vars_x; i++)
	{
		for (uint64_t j = 0; j < tile->num_vars_y; j++,offset++)
		{
			const float cor_val = static_cast<float>(tile->cors[offset]);
			if (cor_val >= cor_threshold_)
			{
				cors_buffer_[num_cors_kept_].var1 = tile->vars_x_ids[i];
				cors_buffer_[num_cors_kept_].var2 = tile->vars_y_ids[j];
				cors_buffer_[num_cors_kept_].val = cor_val;
				num_cors_kept_++;
			}
		}
	}
}


inline void CorrelationsWriter::extract_cors_triangular_tile(Tile* tile)
{
	num_cors_kept_ = 0;
	for (uint64_t i = 0, offset = 1; i < tile->num_vars_x; i++, offset+=i+1)
	{
		for (uint64_t j = i+1; j < tile->num_vars_y; j++,offset++)
		{
			const float cor_val = static_cast<float>(tile->cors[offset]);
			if (cor_val >= cor_threshold_)
			{
				cors_buffer_[num_cors_kept_].var1 = tile->vars_x_ids[i];
				cors_buffer_[num_cors_kept_].var2 = tile->vars_y_ids[j];
				cors_buffer_[num_cors_kept_].val = cor_val;
				num_cors_kept_++;
			}
		}
	}
}

void TextCorrelationsWriter::write_results()
{
	for (uint64_t i = 0; i < num_cors_kept_; i++)
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
		BlockingQueue<Tile*>& tiles_to_write, BlockingQueue<Tile*>& tiles_to_recycle)
{
	if (output_format == TEXT_FORMAT)
	{
		return std::auto_ptr<CorrelationsWriter>(new TextCorrelationsWriter(cors_file, cor_threshold, tiles_to_write, tiles_to_recycle));
	}
	else if (output_format == BINARY_FORMAT)
	{
		return std::auto_ptr<CorrelationsWriter>(new BinaryCorrelationsWriter(cors_file, cor_threshold, tiles_to_write, tiles_to_recycle));
	}
	else
	{
		throw (std::invalid_argument("error: " + output_format+  " is a unknown output format."));
	}
}

