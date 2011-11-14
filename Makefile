CXXFLAGS = -Wall -O2 -pedantic -Wextra `pkg-config --cflags libglfw`
LIBS = -lGLEW -lGLU -lGL `pkg-config --libs libglfw`

all: cw1

cw1: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp shader.cpp -o bin/cw1 $(LIBS)

run: cw1
	./bin/cw1

clean:
	rm -rf *.o cw1
