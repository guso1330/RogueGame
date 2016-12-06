#include "block.h"


block::block()
{
	block_id = 0;     //Initialize new blocks to empty & non-solid
	is_solid = 0;
	is_found = 1;
	has_hostile = 0;
}

void block::set_block_content_id(int new_id)
{
	block_content_id = new_id;
}

void block::set_block_id(int new_id)
{
	block_id = new_id; 
}

int block::get_block_content_id()
{
	return block_content_id;
}

int block::get_block_id()
{
	return block_id;
}