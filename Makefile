CXXFLAGS = -Wall -O2 -pedantic -Wextra `pkg-config --cflags libglfw`
LIBS = -lGLEW -lGLU -lGL `pkg-config --libs libglfw`

all: cw1

cw1: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp common/shader.cpp -o cw1 $(LIBS)

clean:
	rm -rf *.o cw1
