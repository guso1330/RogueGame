#include "entity.h"
#define BASIC_STAT_VALUE 10
#define START_HEALTH 5
#define CON_HEALTH_OFFSET 10
#define START_LEVEL 1
void entity::set_str(int val)
{
	Strength = val;
}

void entity::set_con(int val)
{
	Constitution = val;
}

void entity::set_agi(int val)
{
	Agility = val;
}

void entity::set_p_res(int val)
{
	Physical_Resist = val;
}

void entity::set_e_res(int val)
{
	Energy_Resist = val;
}

void entity::set_level(int lvl)
{
	Level = lvl;
}

void entity::calculate_health()
{
	Health = START_HEALTH + (Constitution - CON_HEALTH_OFFSET);
}

void entity::set_name(std::string unit_name)
{
	name = unit_name;
}

void entity::show_stats()
{
	std::cout << "Name: " << name << std::endl;
	std::cout << "Level: " << Level << std::endl;
	/*for(int counter = 0; counter < Level; ++counter)
	{

	}
		std::cout << "I"; 
	}
	std:: cout << std::endl;
	*/
	std::cout << "Health: " << Health << std::endl;
	std::cout << "Strength: " << Strength << std::endl;
	std::cout << "Constitution: " << Constitution << std::endl;
	std::cout << "Agility: " << Agility << std::endl;
	std::cout << "Physical_Resist: " << Physical_Resist << std::endl;
	std::cout << "Energy_Resist: " << Energy_Resist << std::endl;
	

}
entity::entity()
{
	set_str(BASIC_STAT_VALUE);
	set_con(BASIC_STAT_VALUE);
	set_agi(BASIC_STAT_VALUE);
	set_p_res(BASIC_STAT_VALUE);
	set_e_res(BASIC_STAT_VALUE);
	set_level(START_LEVEL);
	calculate_health();
	set_name("Default Unit Name");

}