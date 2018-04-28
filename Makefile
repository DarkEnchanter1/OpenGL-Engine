LIBPATHS = -L/usr/lib -L/usr/X11R6/lib -L/usr/local/lib
LIBRARIES = -lGL -lGLU -lglut -lGLEW -lglfw -lm -lassimp
CPPFLAGS =  -Wall -I .
all: build run
SRCS= $(shell find . -name "*.cpp")
# This one is done manually, it's for header-only files
HEADERS=common/primitives.hpp
OBJS=$(SRCS:.cpp=.o)
build: $(OBJS) $(HEADERS)
	g++ -Wall -I . $(HEADERS) $(OBJS) $(LIBPATHS) $(LIBRARIES) -o enginetest
run: 
	./enginetest
force:
	g++ -Wall main.cpp object.cpp cube.cpp $Cshader.cpp $Ctexture.cpp $Ccontrols.cpp $Cobjloader.cpp -L/usr/lib -L/usr/X11R6/lib -L/usr/local/lib -lGL -lGLU -lglut -lGLEW -lglfw -lm
