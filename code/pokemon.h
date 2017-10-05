#ifndef POKEMON_H
#define POKEMON_H
#include <string>

class Pokemon {
public:
	Pokemon(int,int,int,int,int,int,bool);
	int id;
	int HP; 
	std::string status;
	int attack; //damage of attack
	int damage; //how many counters are on it
	int cost;
	bool muscle; //true if it holds muscle band
	int energy;	//how many energies it holds
};

#endif