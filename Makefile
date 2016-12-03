# This sample Makefile allows you to make an OpenGL application
#   whose source is exactly one .c file or one .cc file under Solaris.
#   check the location of the X libraries on your implementation.
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.cc is the name of the c++-file you wish to compile 
#  or 
#       xxxx.c is the name of the c-file you wish to compile 
#       
# A binary named xxxx will be produced
# Libraries are assumed to be in the default search paths
# as are any required include files

#
# Modified by Prof. Chelberg to also allow you to make from a single
# c++ source file.  So if xxxx.cc is the sourcefile, you type make
# xxxx to produce the binary file.

# last-modified: Mon Sep 17 15:17:00 2012

# Assumes the following file exists in the proper place.
# include /home/cyrus/Documents/code-egs/Makefile.defs
include /home/guso/Documents/cs4250/code-egs/Makefile.defs


# The first item in the file is what will be made if you just type
# make (i.e. with no arguments).
all: rogueGame
	
rogueGame: rogueGame.cc floor.o block.o object.o mesh.o camera.o objloader.o
	$(CC) rogueGame.cc $(InitShader) *.o $(OPTIONS) $(LDLIBS) -o rogueGame ./bin/libIrrKlang.so ./bin/libSOIL.a -pthread

floor.o: src/GenerationStuff/floor.h src/GenerationStuff/floor.cpp block.o
	$(CC) src/GenerationStuff/floor.cpp -c $(OPTIONS)

block.o: src/GenerationStuff/block.h src/GenerationStuff/block.cpp
	$(CC) src/GenerationStuff/block.cpp -c $(OPTIONS)

object.o: src/object.h src/object.cc mesh.o objloader.o
	$(CC) src/object.cc -c $(OPTIONS)

mesh.o: src/mesh.h src/mesh.cc
	$(CC) src/mesh.cc -c $(OPTIONS)

camera.o: src/camera.h src/camera.cc
	$(CC) src/camera.cc -c $(OPTIONS)

objloader.o: src/objloader.h src/objloader.cc
	$(CC) src/objloader.cc -c $(OPTIONS)

	
# pattern to clean the directory
clean: 
	rm -f rogueGame *.o