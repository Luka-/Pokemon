#include "deck.h"
#include <iostream>
#include <string>
#include <algorithm>

Deck::Deck() {
	deckout = false;
	size = 60;
	for (int i = 0; i < 12; i++) number[i] = 4;
	number[12] = 12;
	for (int i = 0; i < 48; i++) order.push_back(i/4);
	for (int i = 48; i < 60; i++) order.push_back(12);
}

void Deck::shuffle() {
	srand(time(NULL));
	for (int i = 1; i < size; i++) {
		int x = rand()%(i+1);
		int temp = order[i];
		order[i] = order[x];
		order[x] = temp;
	}
}

void Deck::draw(Hand* hand) {
	if (size == 0) deckout = true;
	else {
		int temp = order.back();
		order.pop_back();
		size--;
		number[temp]--;
		hand->number[temp]++;
	}
}

bool Deck::remove(int card) { // removes card if it's in the deck, shuffles it, and returns true; 
							  // otherwise, does nothing and returns false
	if (number[card] == 0) return false;
	number[card]--;
	for (int i = 0; i < size; i++) {
		if (order[i] == card) {
			int temp = order[i];
			order[i] = order[size-1];
			order[size-1] = temp;
			break;
		} 
	}
	order.pop_back();
	size--;
	shuffle();
	return true;
}

int Deck::state() { //template for now
	return 1;
}