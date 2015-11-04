VERSION=LINUX

ifeq "$(VERSION)" "WIN"
# Set compiler to Linux CrossCompile Windows
CXX = i686-w64-mingw32-gcc
# Set Load flags to include SDL
LDFLAGS = `sdl2-config --cflags` -lSDL2 -lSOIL -lopengl32 -lglu32 -lglew32 -lfreeglut
COMPW = -lstdc++
# Set target
EXE = openGLSSVEP.exe
else
# Set compiler to g++
CXX = g++
# Set Load flags to include SDL
LDFLAGS = `sdl2-config --cflags` -lSDL2 -lGL -lGLU -lGLEW -lglut -lSOIL
COMPW = 
# Set target
EXE = openGLSSVEP
endif

# Set comilation flags to include SDL
CPPFLAGS = -std=gnu++11 -Wall $(shell sdl-config --cflags)

# load all .cpp files in directory
SOURCES = $(wildcard *.cpp)

# create object files from source
OBJECTS = $(SOURCES:.cpp=.o)


# g++ -Wall skeletal.cpp $(sdl-config --cflags --libs) -o skeletal
all: $(EXE)
$(info Compiling with $(CXX))
$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(COMPW)


#all: $(SOURCES) $(EXE)
#$(EXE): $(OBJECTS)
#	$(CXX) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $@
#.cpp.o:
#	$(CXX) -c $(SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(EXE)

