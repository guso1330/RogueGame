#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include <ctime>
#include <cstdlib>
#include <string> 
#include "block.h"

using namespace std;

class Floor {
	public: 
		Floor();
		vector <vector <block> > floor_map; 
		int x_dim;
		int y_dim;
	private: 
 
		int num_room_spawners;
		int max_room_size; 
		int total_blocks;

		void generate_dims();
		void calculate_room_spawners();
		void calculate_max_room_size();
		void resize_floor_map();
		void place_room_spawners();
		void connect_rooms(int room_1_x, int room_1_y, int room_2_x, int room_2_y);
		void save_floor(string file_name);
		void generate_room(int x, int y);
		void generate_stairs();
		void generate_objects(int x1, int y1, int x2, int y2);
	};

#endif