#include "field.h"

Field::Field() {
	active = NULL; activeOp = NULL;
	for (int i = 0; i < 5; i++) {
		bench[i] = NULL; benchOp[i] = NULL;
	}
	for (int i = 0; i < 6;i++) prize[i] = -1;
	benchsize = 0; benchsizeOp = 0;
	can_retreat = true;
	prize_counter = 0;
	attached = false;
	for (int i = 0; i < 6; i++) evolved[i] = false;
	supporter = false;
}

int Field::empty_slot() {
	if (benchsize == 5) return -1;
	for (int i = 0; i < 5; i++) {
		if (bench[i] == NULL) return i;
	}
}
// places given ID card on a slot
// if successful, returns 1, if not, does nothing and returns 0
bool Field::place(int id, int slot) { 
									  
	if (slot < 0 || slot > 5) return false;

	if (id == 1) {
		if (slot == 5) {
			if (active == NULL) {
				active = new Pokemon(1,60,0,1,10,0,false);
				evolved[slot] = true;
				return true;
			}
			else return false;
		}
		if (bench[slot] != NULL) return false;
		bench[slot] = new Pokemon(1,60,0,1,10,0,false);
		benchsize++;
		evolved[slot] = true;
		return true;
	}
	else if (id == 2) {
		if (evolved[slot] == true) return false;
		if (slot == 5) {
			if (active == NULL) return false;
			if (active->id != 1) return false;
			active = new Pokemon(2,80,active->damage,3,-1,active->energy,active->muscle);
			evolved[slot] = true;
			return true;
		}
		if (bench[slot] == NULL) return false;
		else if (bench[slot]->id != 1) return false;
		bench[slot] = new Pokemon(2,80,bench[slot]->damage,3,-1,bench[slot]->energy,bench[slot]->muscle);
		evolved[slot] = true;
		return true;
	}
	else if (id == 3) {
		if (evolved[slot] == true) return false;
		if (slot == 5) {
			if (active == NULL) return false;
			if (active->id != 2) return false;
			active = new Pokemon(3,140,active->damage,3,-2,active->energy,active->muscle);
			evolved[slot] = true;
			return true;
		}
		if (bench[slot] == NULL) return false;
		else if (bench[slot]->id != 2) return false;
		bench[slot] = new Pokemon(3,140,bench[slot]->damage,2,-2,bench[slot]->energy,bench[slot]->muscle);
		evolved[slot] = true;
		return true;
	}
	else if (id == 4) {
		if (slot == 5) {
			if (active == NULL) {
				active = new Pokemon(4,110,0,0,0,0,false);
				evolved[slot] = true;
				return true;
			}
			else return false;
		}
		if (bench[slot] != NULL) return false;
		bench[slot] = new Pokemon(4,110,0,0,0,0,false);
		benchsize++;
		evolved[slot] = true;
		return true;
	}

	else return false;
}

bool Field::promote(int card) {
	if (bench[card] == NULL) return false;
	delete active;
	active = bench[card];
	bench[card] = NULL;
	benchsize--;
	return true;
}

bool Field::swap(int card) { //swap active with this card
	if (bench[card] == NULL) return false;
	if (active == NULL) return false;
	Pokemon* temp = active;
	active = bench[card];
	bench[card] = temp;
	return true;
}

int Field::calculate_damage() {
	int dmg = 0;
	if (active == NULL) return 0;
	if (active->energy < active->cost) return 0;
	if (active->attack >= 0) dmg = active->attack;
	int round = 1;
	for (int i = 0; i < 5; i++) {
		if (bench[i] != NULL) {
			if (bench[i]->id == 2 || bench[i]->id == 3 ) {
				round++;
			}
		}
	}
	if (active->id == 2) dmg = 20*round;
	else if (active->id == 3) dmg = 30*round;
	if (active->muscle == true && dmg > 0) dmg += 20;
	return dmg;
}