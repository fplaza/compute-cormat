#include "Parameters.hh"
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

Parameters Parameters::parse(int argc, char* argv[])
{
	Parameters pars;

	// Create options decription
	po::options_description all_parameters("");
	po::options_description input_parameters("Input parameters");
	po::options_description output_parameters("Output parameters");
	po::options_description misc_parameters("Miscellaneous parameters");

	input_parameters.add_options()
		("precomputed-cormat-file,i", po::value<std::string>(&pars.precomputed_cormat_file), "Binary file containing the precomputed correlation matrix.")
		;

	output_parameters.add_options()
		("cors-file,o", po::value<std::string>(&pars.cors_file), "Output file containing the correlations computed.")
		("out-fmt", po::value<std::string>(&pars.output_format)->default_value("binary"), "Format in which correlations are printed ('binary' or 'text')")
		("cor-threshold,t", po::value<float>(&pars.cor_threshold)->default_value(-1.0f,"-1.0"), "Print only correlations above this threshold.")
		;

	misc_parameters.add_options()
		("help,h", "display this help and exit.")
	;

	all_parameters.add(input_parameters).add(output_parameters).add(misc_parameters);

	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(all_parameters).run(), vm);

	// Print help
	if (argc == 1 || vm.count("help"))
	{
		std::cout << all_parameters << std::endl;
		std::exit(0);
	}

	po::notify(vm);

	check_file_is_readable(pars.precomputed_cormat_file);
	check_file_is_writable(pars.cors_file);
	check_val_within_bounds("cor-threshold", pars.cor_threshold, -1.0f, 1.0f);

	return pars;
}

void Parameters::check_file_is_readable(const std::string& filepath)
{
	std::ifstream ifs;
	ifs.open(filepath.c_str());

	if (ifs.good())
	{
		ifs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be opened. Check that the path is valid and that you have read permissions."));
	}
}

void Parameters::check_file_is_writable(const std::string& filepath)
{
	std::ofstream ofs;
	ofs.open(filepath.c_str());

	if (ofs.good())
	{
		ofs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be created. Check that the path is valid and that you have write permissions."));
	}
}

template <typename T>
void Parameters::check_val_within_bounds(const std::string& val_name, T value, T lower, T higher)
{
	if(value < lower || value > higher)
	{
		throw (std::invalid_argument("error: " + val_name + " must be a value within range: " +
					'[' + boost::lexical_cast<std::string>(lower) + ';' + boost::lexical_cast<std::string>(higher) + ']'));
	}
}

