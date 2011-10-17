CXXFLAGS = -Wall -O2
LIBS = -lGLEW -lGLU -lGL `pkg-config --libs libglfw`

all: cw1

cw1: main.o
	$(CXX) `pkg-config --cflags libglfw` main.o -o cw1 $(LIBS)

clean:
	rm -rf *.o cw1
