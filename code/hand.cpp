#include "deck.h"


Hand::Hand() {
	size = 0;
	for (int i = 0; i < 13; i++) number[i] = 0;
}

bool Hand::discard(int card) {
	if (number[card] == 0) return false;
	number[card]--;
	size--;
	return true;
}

bool Hand::add_from_deck(int card, Deck* deck) {
	if (!deck->remove(card)) return false;
	add_from_void(card);
	return true;
}

void Hand::add_from_void (int card) {
	size++;
	number[card]++;
}

bool Hand::shuffleIn(int card, Deck* deck) {
	if (number[card] == 0) return false;
	number[card]--;
	deck->number[card]++;
	size--;
	deck->size++;
	deck->order.push_back(card);
	deck->shuffle();
	return true;
}

bool Hand::play(int card, Field* field, Deck* deck, int arg1, int arg2) { // most important function, denotes the play
	if (number[card] == 0) return false;
	if (number[card] < 1) return false;

	if (card == 0) { //evosoda, arg1 pokemon id in deck, arg2 position on the field
		if (field->evolved[arg2] == true) return false;
		if (arg1 != 2 && arg1 != 3) return false;
		if (deck->number[arg1] > 0) {
			if (field->place(arg1,arg2)) {
				bool temp = deck->remove(arg1);
				number[card]--;
				size--;
				return true;
			}
		}
	}

	if (card == 1) { //puts Tympole on the bench
		if (arg1 != 0 || arg2 != 0) return false;
		if (field->place(1,field->empty_slot())) {
			number[card]--;
			size--;
			return true;
		}
	}

	if (card == 2) { // evolves Tympole on position arg1
		if (arg2 != 0) return false;
		if (field->place(2,arg1)) {
			number[card]--;
			size--;
			return true;
		}
	}

	if (card == 3) { // evolves Palpitoad on position arg1
		if (arg2 != 0) return false;
		if (number[3] < 1) return false;
		if (field->place(3,arg1)) {
			number[card]--;
			size--;
			return true; 
		}
	}

	if (card == 4) { //puts Meloetta on the bench 
		if (arg1 != 0 || arg2 != 0) return false;
		if (field->place(4,field->empty_slot())) {
			number[card]--;
			size--;
			return true;
		}
	}

	if (card == 5) { // Muscle Band, arg1 is bench slot
		if (arg2 != 0) return false;
		if (arg1 < 0 || arg1 > 5) return false;
		if (arg1 == 5) {
			if (field->active == NULL) return false;
			if (field->active->muscle == true) return false;
			field->active->muscle = true;
			return true;
		}
		else {
			if (field->bench[arg1] == NULL) return false;
			if (field->bench[arg1]->muscle == true) return false;
			field->bench[arg1]->muscle = true;
			number[card]--;
			size--;
			return true;
		}
	}

	if (card == 6) { //Cheren
		if (field->supporter) return false;
		if (arg1 != 0 || arg2 != 0) return false;
		for (int i = 0; i < 3; i++) deck->draw(this);
			number[card]--;
			size--;
			field->supporter = true;
			return true;
	}

	if (card == 7) { //Juniper 
		if (field->supporter) return false;
		if (arg1 != 0 || arg2 != 0) return false;
		for (int i = 0; i < 13; i++) number[i] = 0;
		for (int i = 0; i < 7; i++) deck->draw(this);
		size = 7;
	field->supporter = true;
	return true;
	}

	if (card == 8) { //heavy ball, arg1 is id for Pokemon
		if (arg2 != 0) return false;
		if (arg1 != 3) return false;
		if (deck->remove(arg1)) {
			number[card]--;
			number[arg1]++;
			return true;
		}
		

	}

	if (card == 9) { // Level Ball, arg1 is id for Pokemon
		if (arg2 != 0) return false;
		if (arg1 != 1 && arg1 != 2) return false;
		if (deck->remove(arg1)) {
			number[9]--;
			number[arg1]++;
			return true;
		}
	}

	if (card == 10) { // Pokemon Communication, arg1 id for receiving, arg2 id for the one going back
		if (arg1 != 1 && arg1 != 2 && arg1 != 3 && arg1 != 4) return false;
		if (arg2 != 1 && arg2 != 2 && arg2 != 3 && arg2 != 4) return false;
		if (number[arg2] < 1) return false;
		if (deck->number[arg1] < 1) return false;
		deck->number[arg2]++;
		deck->number[arg1]--;
		number[arg1]++;
		number[arg2]--;
		number[card]--;
		size--;
		return true;
	}

	if (card == 11) { //DCE, arg1 is bench/active slot
		if (arg2 != 0) return false;
		if (field->attached) return false;
		if (arg1 < 0 || arg1 > 5) return false;
		if (arg1 == 5 && field->active != NULL) {
			field->active->energy += 2;
			size--;
			number[card]--;
			field->attached = true;
			return true;
		}
		else {
			if (field->bench[arg1] != NULL) {
				field->bench[arg1]->energy += 2;
				number[card]--;
				size--;
				field->attached = true;
				return true;
			}
		}
	}

	if (card == 12) { //Energy, arg1 is bench/active slot
		if (arg2 != 0) return false;
		if (field->attached) return false;
		if (arg1 < 0 || arg1 > 5) return false;
		if (arg1 == 5 && field->active != NULL) {
			field->active->energy += 1;
			size--;
			number[card]--;
			field->attached = true;
			return true;
		}
		else {
			if (field->bench[arg1] != NULL) {
				field->bench[arg1]->energy += 1;
				number[card]--;
				size--;
				field->attached = true;
				return true;
			}
		}
	}	

	if (card == 13) { //retreat, arg1 is bench slot
		if (arg2 != 0) return false;
		if (field->active == NULL) return false;
		if (field->active->id != 4) return false;
		if (field->bench[arg1] == NULL) return false;
		if (field->active->energy < 2) return false;
		field->active->energy -= 2;
		return field->swap(arg1);
	}
	
	return false;
	
}

void Hand::take_prize(Field* field) { // simply takes the next prize
	number[field->prize[field->prize_counter]]++;
	field->prize_counter++;
	size++;
}

void Hand::shuffleHand (Deck* deck) {
	for (int i = 0; i < 13; i++) {
		while (number[i] > 0) {
			shuffleIn(i,deck);
		}
	}
}

int Hand::state() { //template 
	return 1;
}

