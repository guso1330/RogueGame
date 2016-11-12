# Where to find the include files
INCS = -I/usr/local/include/

# Compiler options -g for debug info
GCC_OPTIONS= -pedantic -g

# What libraries to link GLU is included for backwards compatibility
LFLIB = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi /usr/lib/x86_64-linux-gnu/libGLEW.so.1.13

# Options parameter to pass to the compiler
OPTIONS = $(GCC_OPTIONS) $(INCS)

all: main.o
	g++ main.o $(OPTIONS) $(LDLIBS) -o main

main.o:
	g++ -std=c++11 -c main.cpp

clean:
	rm main *.o