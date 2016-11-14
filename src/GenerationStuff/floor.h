#include <vector>
#include <ctime>
#include <cstdlib>
#include "block.h"
using namespace std;
class floor {
public: 
	floor();
	vector <vector <block> > floor_map; 


private: 
	int x_dim;
	int y_dim; 
	int num_room_spawners;
	int max_room_size; 
	int total_blocks;

	void generate_dims();
	void calculate_room_spawners();
	void calculate_max_room_size();
	void resize_floor_map();
	void place_room_spawners();
	void connect_rooms(int room_1_x, int room_1_y, int room_2_x, int room_2_y);
};