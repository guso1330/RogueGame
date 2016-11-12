#
# Makefile for the RogueGame
#
# Created by Gus Oberdick (Nov. 12 3:33pm)
# Last modified by (___)
#


# Where to find the include files
INCS = -I/usr/local/include/

# Compiler options -g for debug info
GCC_OPTIONS= -pedantic -g

# What libraries to link GLU is included for backwards compatibility
# LDLIBS = /usr/lib/x86_64-linux-gnu/libGLEW.so.1.13 -lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread
LDLIBS = /usr/lib/x86_64-linux-gnu/libGLEW.so.1.13 -lglfw3 -lGL -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread
#LDLIBS = /usr/lib/x86_64-linux-gnu/libGLEW.so.1.13 -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -lpthread -lm -lrt -ldl

# Options parameter to pass to the compiler
OPTIONS = $(GCC_OPTIONS) $(INCS)

CC = g++ -std=c++11

all: game

game: main.cpp window.o
	$(CC) main.cpp window.o $(OPTIONS) $(LDLIBS) `pkg-config --cflags glfw3` -o game `pkg-config --static --libs glfw3`

window.o: src/window.h src/window.cpp
	$(CC) src/window.cpp -c $(OPTIONS)

clean:
	rm game *.o