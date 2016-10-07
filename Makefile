EXEC = compute-cormat

CC = g++
CFLAGS = -Wall -Wextra -O3 -march=native -fopenmp 
BOOST_LIBS = -lboost_program_options -lboost_system -lboost_thread
OPEN_BLAS_LIBS = -lopenblas
LDFLAGS = $(BOOST_LIBS) ${OPEN_BLAS_LIBS} -fopenmp 

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
