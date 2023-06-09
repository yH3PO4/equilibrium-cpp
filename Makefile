CPP = g++
CPPFLAGS = -O3 -Wall -Wunused-parameter -Wno-unused-result -Wno-sign-compare -fopenmp -std=c++17
SRCS = $(wildcard src/*.cpp)
PROGRAM = main

$(PROGRAM): $(SRCS)
	$(CPP) $(CPPFLAGS) $(SRCS) -o $(PROGRAM)
