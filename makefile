CXX=g++
CXX_FLAGS= -std=c++17 -O2 -I ./armadillo-9.600.5/include
LD_FLAGS= -framework Accelerate

.PHONY: cleanall

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^

qp-solver: Problem.o Projection.o Utils.o main.o
	$(CXX) $(LD_FLAGS) $^ -o qp-solver

all: qp-solver

cleanall:
	-rm -f *.o qp-solver
