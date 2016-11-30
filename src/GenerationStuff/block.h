#ifndef BLOCK_H
#define BLOCK_H
//_________________________________________________________________
/*	Author: Cyrus Ditchendorf & Gus Oberdick
|*	Class:	block
|*	Purpose: The class for a single unit of area in the game. 
|*
|*	Notes: 
|*		Block IDs: 
|*				0 = Empty
|* 				1 = Room Spawner Block
|*				2 = Floor
|*				3 = Tile Floor
|*				4 = Wood Wall
|*				5 = Stone Wall
|*				6 = Tile Wall
|*				10 = Entry Stairs
|* 				11 = Exit Stairs
|*		Block Content IDs: 
|*				0 = Empty
|*				1 = Table
|*				2 = Chair 
|*				3 = Chest 
|*				4 = Trap (Might do something cool with this.)
|_________________________________________________________________*/

class block {
public: 
	block();
	int get_block_id();
	void set_block_id(int new_id);  //Sets the block id

	bool is_found;  //if the block has been found by the player
 
private: 
	int block_id; //A number corresponding to the type of block
	int block_content_id; // A number corresponding to what the block contains (decorational items)
	bool is_solid; //Can the player pass through this block? Depends on what the block's ID is and if it contains an object.


};

#endif