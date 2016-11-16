/*
|*	Author: Cyrus Ditchendorf & Gus Oberdick
|*	Class: entity
|*	Purpose: Basic class that all moving characters in the
|* 			 game derive from.
|*	Notes: 
|* 		Stats: 
|*			Strenght: 
|*				*How hard character hits with strengh based weapons.)
|*			Constitution:
|*				*Determines Health
|*					*Health = 5 + (Constitution - 10)
|*			Agility: 
|*				*Determines how hard character hits with ranged
|* 				 weapons and the accuracy. 
|*			Physical_Resist:
|*				*Resistance to strength based weapons. 
|*			Energy_Resist:
|*				*Resistance to Agility based weapons.
*/
#include <iostream>
#include <string> 
class entity
{
public:
	entity();
	int get_health();
	void show_stats();

private: 
	int Strength;
	int Constitution;
	int Agility; 
	int Physical_Resist;
	int Energy_Resist; 
	int Level;
	int Health;
	std::string name; 

	void set_str(int str);
	void set_con(int con);
	void set_agi(int agi);
	void set_p_res(int p_res);
	void set_e_res(int e_res);
	void set_level(int lv);
	void calculate_health();
	void set_name(std::string unit_name);
};