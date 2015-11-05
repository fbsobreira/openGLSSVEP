VERSION=LINUX

ifeq "$(VERSION)" "WIN"
# Set compiler to Linux CrossCompile Windows
CXX = i686-w64-mingw32-gcc
# Set Load flags to include SDL
LDFLAGS = -lSOIL -lopengl32 -lglu32 -lglew32 -lfreeglut
COMPW = -lstdc++
# Set target
EXE = openGLSSVEP.exe
FREETYPE_INCLUDE = /usr/include/freetype2
else
# Set compiler to g++
CXX = g++
# Set Load flags to include SDL
LDFLAGS = -lGL -lGLU -lGLEW -lglut -lSOIL -lglfw -lfreetype
COMPW = 
# Set target
EXE = openGLSSVEP
FREETYPE_INCLUDE = /usr/include/freetype2
endif

# Set comilation flags to include SDL
CPPFLAGS = -std=gnu++11 -Wall -I$(FREETYPE_INCLUDE)
# load all .cpp files in directory
SOURCES = $(wildcard *.cpp)

# create object files from source
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXE)
$(info Compiling with $(CXX))
$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(COMPW)

clean:
	rm -f $(OBJECTS) $(EXE)

