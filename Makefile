LIBS=GLEW GLU
LD_FLAGS+=$(addprefix -l, $(LIBS)) `pkg-config --libs libglfw`

CXXFLAGS = -Wall -O2 -pedantic -Wextra `pkg-config --cflags libglfw`
MYLIBS = shader utils mesh renderer
MYOBJECTS = $(addsuffix .o, $(MYLIBS)) 

all: comp3004

comp3004: main.cpp libs.a
	$(CXX) $(CXXFLAGS) -o bin/comp3004 main.cpp bin/libs.a $(LD_FLAGS)

libs.a: $(MYOBJECTS)
	ar rcs bin/$@ $(addprefix bin/,$(MYOBJECTS))

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o bin/$@

run: comp3004
	bin/comp3004

.PHONY: clean
clean:
	rm -rf bin/
