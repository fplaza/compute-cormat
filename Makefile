EXEC = compute-cormat

CC = /opt/intel/bin/icc
MKL_INCLUDE =/opt/intel/composer_xe_2013_sp1.2.144/mkl/include/
CFLAGS = -I${MKL_INCLUDE} -Wall -Wextra -O3 -march=native -fopenmp 
BOOST_LIBS = -lboost_program_options -lboost_thread-mt -lboost_iostreams
MKL_LIBS_DIR =  /opt/intel/composer_xe_2013_sp1.2.144/mkl/lib/intel64/
MKL_LIBS = -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core
LDFLAGS = -L$(MKL_LIBS_DIR) $(BOOST_LIBS) $(MKL_LIBS) -fopenmp 

SRC = $(wildcard *.cc)
OBJECTS = $(patsubst %.cc, %.o, $(SRC))

.PHONY : clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) *.o
