#include "floor.h"
#include <iostream>
#include <fstream>
#include <utility>

//
// Macros
//
#define SPAWNER_DENOM 95 //was 65
#define DIM_SLACK 35 //was 35
#define DIM_MIN 24  //was 15
#define ROOM_SIZE_SCALE .0014
#define ROOM_SPAWNER_PADDING 8
#define OBJECT_DENSITY .15 
#define HOSTILE_DENSITY .05

//
// Block Content Macros
//
#define EMPTY 0
#define CHAIR 1
#define TABLE 2
#define SCRAPS 3
#define RUBBLE 4
#define BONFIRE 5
#define NUM_ITEMS 5  //number of items excluding EMPTY

void Floor::generate_dims() {
	srand(time(NULL)); // Seed random time



}