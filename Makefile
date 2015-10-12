# Set compiler to g++
CXX = g++

# Set comilation flags to include SDL
CPPFLAGS = -Wall $(shell sdl-config --cflags)
#CPPFLAGS =

# Set Load flags to include SDL
#LDFLAGS = $(shell sdl-config --libs)
LDFLAGS = `sdl2-config --cflags` -lSDL2 -lGL -lGLU -lGLEW -lglut

# load all .cpp files in directory
SOURCES = $(wildcard *.cpp)

# create object files from source
OBJECTS = $(SOURCES:.cpp=.o)

# Set target
EXE = ssvep

# g++ -Wall skeletal.cpp $(sdl-config --cflags --libs) -o skeletal
all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $@



#all: $(SOURCES) $(EXE)
#$(EXE): $(OBJECTS)
#	$(CXX) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $@
#.cpp.o:
#	$(CXX) -c $(SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(EXE)

