#ifndef DECK_H
#define DECK_H
#include <vector>
#include "hand.h"

class Deck {
public:
	Deck();
	int size;
	bool deckout;
	int number[13];
	std::vector <int> order;
	
	void shuffle();
	void draw(Hand*);
	bool remove(int);
	
	int state();

};

#endif