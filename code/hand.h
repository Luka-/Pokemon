#ifndef HAND_H
#define HAND_H
#include <iostream>
#include <string>
#include <vector>
#include "deck.h"
#include "field.h"

class Deck;
class Field;

class Hand {
	public: 
		Hand();
		int size;
		int number[13];
		
		bool discard(int);
		bool add_from_deck(int, Deck*);
		void add_from_void(int); 
		bool shuffleIn(int,Deck*);
		void shuffleHand(Deck*);
		bool play(int, Field*, Deck*, int, int);
		void take_prize(Field*);
		
		int state();
};

#endif