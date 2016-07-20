#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include "deck.h"
#include "pokemon.h"

class Pokemon;

class Field {
public:
	Field();

	Pokemon* active;
	Pokemon* activeOp;
	Pokemon* bench[5];
	Pokemon* benchOp[5];
	bool supporter;

	int prize[6];
	int prize_counter;

	int benchsize, benchsizeOp;
	bool can_retreat;
	bool attached;
	bool evolved[6];

	bool place(int,int);
	int empty_slot();
	bool promote(int);
	bool swap(int);
	int calculate_damage();
};


#endif