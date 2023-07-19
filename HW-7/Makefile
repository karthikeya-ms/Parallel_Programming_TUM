CXX = mpicxx
CXX_FLAGS = --std=c++17 -Wall -Wextra -mavx -O3 -g -DOMPI_SKIP_MPICXX
# this compiler definition is needed to silence warnings caused by the openmpi CXX
# bindings that are deprecated. This is needed on gcc 8 forward.
# see: https://github.com/open-mpi/ompi/issues/5157

all: env_file sequential_implementation student_submission

Utility.o: Utility.cpp Utility.h
	$(CXX) $(CXX_FLAGS) -c -o Utility.o Utility.cpp

sequential_implementation: StringSearch.h Utility.o sequential_implementation.cpp
	$(CXX) $(CXX_FLAGS) -o sequential_implementation Utility.o sequential_implementation.cpp

env_file: student_submission.cpp
	sed -n -e 's/.*!submission_env \(.*\)/\1/p' student_submission.cpp > student_submission.env

student_submission: StringSearch.h Utility.o student_submission.cpp
	$(CXX) $(CXX_FLAGS) -o student_submission Utility.o student_submission.cpp

clean:
	rm -f sequential_implementation student_submission student_submission.env

