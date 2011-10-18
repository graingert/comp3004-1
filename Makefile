CXXFLAGS = -Wall -O2
LIBS = -lGLEW -lGLU -lGL `pkg-config --libs libglfw`

all: cw1

cw1: main.cpp
	$(CXX) `pkg-config --cflags libglfw` main.cpp common/shader.cpp -o cw1 $(LIBS)

clean:
	rm -rf *.o cw1
