#include "pokemon.h"


Pokemon::Pokemon(int i, int h, int d, int c, int a, int e, bool m) {
	id = i;
	HP = h;
	damage = d;
	status = "normal";
	cost = c;
	attack = a;
	muscle = m;
	energy = e;
}