#include "floor.h"
#include <iostream>
#include <fstream>
#include <utility>
#define SPAWNER_DENOM 95 //was 65
#define DIM_SLACK 35 //was 35
#define DIM_MIN 20  //was 15
#define ROOM_SIZE_SCALE .0014
#define ROOM_SPAWNER_PADDING 4

void Floor::generate_dims()
{
	srand(time(NULL));
	x_dim = rand() % DIM_SLACK + DIM_MIN;		//from size 15 to 40
	y_dim = rand() % DIM_SLACK + DIM_MIN;

	std::cout << "X-dim = " << x_dim << std::endl;
	std::cout << "Y-dim = " << y_dim << std::endl;
}

void Floor::calculate_room_spawners()
{
	total_blocks = x_dim * y_dim;
	std::cout << "Total Blocks = " << total_blocks << std::endl;
	num_room_spawners = total_blocks / SPAWNER_DENOM; 
	std:: cout << "Total Room Spawners = " << num_room_spawners << std::endl;
}

void Floor::calculate_max_room_size()
{
	max_room_size = total_blocks * ROOM_SIZE_SCALE + 2;
	std::cout << "Max Room Size = " << max_room_size << std::endl;
}

void Floor::resize_floor_map()
{
	floor_map.resize(x_dim);
	for(int i = 0; i < x_dim; ++i)
		floor_map[i].resize(y_dim);
}

void Floor::connect_rooms(int x1, int y1, int x2, int y2)
{
	//determine more left room
	int hallway_x, hallway_y;
	int hallway_x_end, hallway_y_end;
	if(x1 < x2)
	{
		hallway_x = x1;
		hallway_x_end = x2;
	} else {
		hallway_x = x2; 
		hallway_x_end = x1;
	}
	//generate horizontal hallway segment
	//std::cout << "X-Hallway Generation." << std::endl;
	for(;hallway_x <= hallway_x_end; ++hallway_x)
	{
		//set to floor
		//std::cout <<"setting [" << hallway_x << "][" << y1 << "] to floor"<< std::endl;
		if(floor_map[hallway_x][y1].get_block_id() != 3)
			if(floor_map[hallway_x][y1].get_block_id() != 1)
				floor_map[hallway_x][y1].set_block_id(2);
	}

	//determine bottom most room
	if(y1 < y2)
	{
		hallway_y = y1;
		hallway_y_end = y2;
	} else {
		hallway_y = y2;
		hallway_y_end = y1;
	}
	//std::cout << "Y-Hallway Generation." << std::endl;
	for(;hallway_y < hallway_y_end; ++hallway_y)
	{
		//std::cout <<"setting [" << hallway_y << "][" << x1 << "] to floor"<< std::endl;
		if(floor_map[x2][hallway_y].get_block_id() != 3)
			if(floor_map[x2][hallway_y].get_block_id() != 1)
				floor_map[x2][hallway_y].set_block_id(2);
	}
}

void Floor::generate_room(int x, int y)
{
	//size of this room
	int this_room_size;
	this_room_size = rand() % (max_room_size - 1) + 1;
	for(int x_counter = x - this_room_size; x_counter < (x+this_room_size); ++x_counter)
	{
		for(int y_counter = y - this_room_size ; y_counter < (y + this_room_size); ++y_counter)
		{
			//make sure its in playable bounds
			if((x_counter > 0)&&(x_counter < x_dim)){
				if((y_counter > 0)&&(y_counter < y_dim)){
					if(floor_map[x_counter][y_counter].get_block_id() != 1)
						floor_map[x_counter][y_counter].set_block_id(3);
				}
			}
		}
	}

}

void Floor::place_room_spawners()
{
	std::cout << "Placing room spawners." << std::endl;
	int room_x, room_y;
	int last_room_x, last_room_y;	
	for(int room_counter = 0; room_counter < num_room_spawners; room_counter ++)
	{
		if(room_counter > 0)
		{
			last_room_x = room_x;
			last_room_y = room_y;
		}
		//room_x = rand() % x_dim;
		//room_y = rand() % y_dim;
		room_x = rand() % x_dim;
		//replace if it falls into the padded zone.
		if(room_x < (ROOM_SPAWNER_PADDING))
		{
			room_x = room_x + ROOM_SPAWNER_PADDING;
		}
		if(room_x > (x_dim - ROOM_SPAWNER_PADDING))
		{
			room_x = room_x - ROOM_SPAWNER_PADDING;
		}
		room_y = rand() % y_dim;
		if(room_y < (ROOM_SPAWNER_PADDING))
		{
			room_y = room_y + ROOM_SPAWNER_PADDING;
		}
		if(room_y > (y_dim - ROOM_SPAWNER_PADDING))
		{
			room_y = room_y - ROOM_SPAWNER_PADDING;
		}
		//std::cout << "Room #" << room_counter << " placed at ("
		//<< room_x << "," << room_y << "). " << std::endl;
		floor_map[room_x][room_y].set_block_id(1); 
		//std::cout << "floor_map[" << room_x << "][" << room_y << "] set to spawner block." << std::endl;
		generate_room(room_x, room_y);
		if(room_counter > 0){
			//std::cout << "Connecting room #" << room_counter-1 << " to #" << room_counter << ". " << std::endl;
			//std::cout << "Last room at [" << last_room_x << "][" << last_room_y << "] to";
			//std::cout << "current room at [" << room_x << "][" << room_y << "]." << std::endl;
				connect_rooms(last_room_x, last_room_y, room_x, room_y);

		}
	}

}

void Floor::save_floor(string name)
{
	ofstream myFile;
	myFile.open("./output.txt");

	for(int i = 0; i < y_dim; i++)
	{
		for(int j = 0; j < x_dim; j++)
		{
			int cur_id; 
			cur_id = floor_map[j][i].get_block_id();
			
			switch(cur_id)
			{
				case 0:
					myFile << "▓";
					//myFile << "0";
					break;
				case 1: 
					myFile << "⧫";
					//myFile << "1";
					break;
				case 2:
					myFile << "░";
					//myFile << "2";
					break;
				case 3:
					myFile << "█";
					//myFile << "3";
					break;
				case 10:
					myFile << "S";
					break;
				case 11:
					myFile << "E";
					break;
			}
			//myFile << floor_map[j][i].get_block_id();
		}
		myFile << "\n";
	}
	myFile.close();
}

void Floor::generate_stairs()
{
	//find room generator nodes
	vector<int> x_vec;
	vector<int> y_vec;

	for(int i = 0; i < x_dim; ++i)
	{
		for(int j = 0; j < y_dim; ++j)
		{
			if(floor_map[i][j].get_block_id() == 1)
			{
				x_vec.push_back(i);
				y_vec.push_back(j);
			}
		}
	}

	int size; 
	size = x_vec.size();
	int start_stair;
	int end_stair; 
	start_stair = rand() % size;
	end_stair = rand() % size;
	while(start_stair == end_stair)
		end_stair = rand() % size;
	//place entry stairs

	floor_map[x_vec[start_stair]][y_vec[start_stair]].set_block_id(10);

	//place exit stairs
	floor_map[x_vec[end_stair]][y_vec[end_stair]].set_block_id(11);
}
Floor::Floor()
{
	generate_dims();
	calculate_room_spawners();
	calculate_max_room_size();
	resize_floor_map();
	place_room_spawners();
	generate_stairs();
	save_floor("output");
}