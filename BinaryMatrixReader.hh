#ifndef BINARY_MATRIX_READER_HH
#define BINARY_MATRIX_READER_HH

#include "Matrix.hh"
#include <string>

class BinaryMatrixReader
{
    public:
        static Matrix read(const std::string& input_file);
};

#endif
