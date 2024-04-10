CXX = clang++
LDFLAGS = -L/opt/homebrew/lib/
LDLIBS = -lSDL2
CXXFLAGS = -Wall -std=c++17 -I/opt/homebrew/include/

objects = main.o dsq.o vid.o

.PHONY: all clean

all: plasma

clean:
	rm *.o plasma

plasma: $(objects)
	$(CXX) $(LDFLAGS) $(LDLIBS) $(objects) -o plasma

dsq.o: dsq.cpp dsq.h

vid.o: vid.cpp vid.h

main.o: main.cpp vid.h dsq.h
