#include <iostream>
#include <algorithm>
#include "deck.h"
using namespace std;

struct Triple {
	int first;
	int second;
	int third;
};

vector<Triple> moves;
double reward[10000];

void find_available_moves(Deck* deck, Hand* hand, Field* field) {
	moves.clear();
	Triple pass;
	pass.first = -1; pass.second = 0; pass.third = 0;
	moves.push_back(pass);
	for (int coord1 = 0; coord1 < 14; coord1++) {
		for (int coord2 = 0; coord2 < 13; coord2++) {
			for (int coord3 = 0; coord3 < 13; coord3++) {
				Deck tempDeck = Deck();
				Hand tempHand = Hand();
				Field tempField = Field();

				for (int i = 0; i < 13; i++) {
					tempHand.number[i] = hand->number[i];
				}
				tempHand.size = hand->size;

				tempDeck.size = deck->size;
				for (int i = 0; i < 13; i++) {
					tempDeck.number[i] = deck->number[i];
				}
				for (int i = 0; i < deck->order.size(); i++) {
					tempDeck.order.push_back(deck->order[i]);
				}
				tempDeck.deckout = deck->deckout;

	 			if (field->active != NULL) {
					tempField.active = new Pokemon(field->active->id,field->active->HP,field->active->damage,field->active->cost,field->active->attack,field->active->energy,field->active->muscle);
				}
				for (int i = 0; i < 5; i++) {
					if (field->bench[i] != NULL) {
						tempField.bench[i] = new Pokemon(field->bench[i]->id,field->bench[i]->HP,field->bench[i]->damage,field->bench[i]->cost,field->bench[i]->attack,field->bench[i]->energy,field->bench[i]->muscle);
					}
				} 
				tempField.can_retreat = field->can_retreat;
				tempField.supporter = field->supporter;
				tempField.attached = field->attached;
				tempField.benchsize = field->benchsize;
				tempField.prize_counter = field->prize_counter;
				for (int i = 0; i < 6; i++) tempField.prize[i] = field->prize[i];
				for (int i = 0; i < 6; i++) tempField.evolved[i] = field->evolved[i];

				Triple temp;

				//here you generate moves
				bool canPlay = tempHand.play(coord1,&tempField,&tempDeck,coord2,coord3);
				if (canPlay) {
					temp.first = coord1;
					temp.second = coord2;
					temp.third = coord3;
					moves.push_back(temp);
				}


				//delete stuff
				delete tempField.active;
				for (int i = 0; i < 5; i++) delete tempField.bench[i];

			}
		}
	} 
}

// simulates random Monte Carlo games
void simulate_game(Deck* deck, Hand* hand, Field* field, int move1, int move2, int move3, int index) {
	int turn = 0;
	Deck tempDeck = Deck();
	Hand tempHand = Hand();
	Field tempField = Field();

	for (int i = 0; i < 13; i++) {
		tempHand.number[i] = hand->number[i];
	}
	tempHand.size = hand->size;

	tempDeck.size = deck->size;
	for (int i = 0; i < 13; i++) {
		tempDeck.number[i] = deck->number[i];
	}
	for (int i = 0; i < deck->order.size(); i++) {
		tempDeck.order.push_back(deck->order[i]);
	}
	tempDeck.deckout = deck->deckout;

	 if (field->active != NULL) {
		tempField.active = new Pokemon(field->active->id,field->active->HP,field->active->damage,field->active->cost,field->active->attack,field->active->energy,field->active->muscle);
	}
	for (int i = 0; i < 5; i++) {
		if (field->bench[i] != NULL) {
			tempField.bench[i] = new Pokemon(field->bench[i]->id,field->bench[i]->HP,field->bench[i]->damage,field->bench[i]->cost,field->bench[i]->attack,field->bench[i]->energy,field->bench[i]->muscle);
		}
	} 
	tempField.can_retreat = field->can_retreat;
	tempField.supporter = field->supporter;
	tempField.attached = field->attached;
	tempField.benchsize = field->benchsize;
	tempField.prize_counter = field->prize_counter;
	for (int i = 0; i < 6; i++) tempField.prize[i] = field->prize[i];
	for (int i = 0; i < 6; i++) tempField.evolved[i] = field->evolved[i];

	tempDeck.shuffle();
	tempHand.play(move1,&tempField,&tempDeck,move2,move3);
	
	// this is where simulation is

	while (turn < 3) {
		find_available_moves(&tempDeck,&tempHand,&tempField);
		srand(time(NULL));
		int random = rand()%moves.size();

		// THIS IS WHERE THE MONEY IS AT
		
		if (moves[random].first == -1) {
			turn++;
			tempDeck.draw(&tempHand);
			tempField.can_retreat = false;
			tempField.attached = false;
			for (int i = 0; i < 6; i++) tempField.evolved[i] = false;
			reward[index] += tempField.calculate_damage();
		 //	cout << "Active is: " << tempField.active->id << " and it does " << tempField.calculate_damage() << " damage." << endl;
		}

		else {
			tempHand.play(moves[random].first,&tempField,&tempDeck,moves[random].second,moves[random].third);
		}
	}

	delete tempField.active;
	for (int i = 0; i < 5; i++) delete tempField.bench[i];
}

// this function diplays the game state on the terminal
void display(Deck* deck, Hand* hand, Field* field) {
	cout << "Your playing field is:" << endl;
	if (field->active != NULL) cout << field->active->id << endl;
	else cout << "0" << endl;
	for (int i = 0; i < 5; i++) {
		if (field->bench[i] != NULL) cout << field->bench[i]->id << " "; 
		else cout << "0" << " ";
	}
	cout << endl;
	for (int i = 0; i < 5; i++) {
		if (field->bench[i] != NULL) cout << field->bench[i]->energy << " "; 
		else cout << "0" << " ";
	}
	cout << endl;
	cout << endl << "Your hand is:" << endl;
	for (int i = 0; i < 13; i++) {
		int temp = hand->number[i];
		while (temp > 0) {
			cout << i << " ";
			temp--;
		}
	} 
	cout << endl;
}


void set_prizes(Deck* deck, Field* field) {
	for (int i = 0; i < 6; i++) {
		field->prize[i] = deck->order.back();
		deck->order.pop_back();
		deck->size--;
		deck->number[field->prize[i]]--;
	} 
}

void play_a_game() { //this plays a game where I control what I draw, i.e. for PTCGO purposes

	bool discovered = false; //tells me whether I've looked in the deck and figured out prizes

	Deck deck = Deck();
	Hand hand = Hand();
	Field field = Field();
	deck.shuffle();
//	set_prizes(&deck,&field);
	
	// this part sets up the beginning of the game

	cout << "Enter first 7 drawing cards:" << endl;
	for (int i = 0; i < 7; i++) {
		int num; cin >> num;
		if (num < 0 || num > 12) {
			cout << "Card doesn't exist, type again: " << endl;
			i--;
			continue;
		}
		hand.add_from_deck(num,&deck);
	}

	cout << "Enter number of extra drawing cards:" << endl;
	int mulligan; cin >> mulligan;
	if (mulligan > 0) cout << "Enter those cards" << endl;
	for (int i = 0; i < mulligan; i++) {
		int num; cin >> num;
		if (num < 0 || num > 12) {
			cout << "Card doesn't exist, type again: " << endl;
			i--;
			continue;
		}
		hand.add_from_deck(num,&deck);
	}

	cout << "Choose your active Pokemon:" << endl;
	int activenum = 0;
	while ((activenum != 1 && activenum != 4) || hand.number[activenum] == 0) {
		cin >> activenum;
	}

	field.place(activenum,5);
	hand.number[activenum]--;

	
	while(true) {

		//this will calculate moves by using Monte Carlo

		for (int i = 0; i < 10000; i++) reward[i] = 0;
		find_available_moves(&deck,&hand,&field);
		vector<Triple> current_moves;
		for (int i = 0; i < moves.size(); i++) current_moves.push_back(moves[i]);
		for (int i = 0; i < current_moves.size(); i++) {
			for (int j = 0; j < 5; j++) {
				simulate_game(&deck,&hand,&field,current_moves[i].first,current_moves[i].second,current_moves[i].third,i);
			}
		}

		

		display(&deck,&hand,&field);

		cout << endl;
		cout << "Your available moves are:" << endl;
		for (int i = 0; i < current_moves.size(); i++) {
			cout << current_moves[i].first << " " << current_moves[i].second << " " << current_moves[i].third << " Damage: " << reward[i] << endl;
		}

		cout << "Enter your move:" << endl;

		//this is the code that will actually handle the move

		int a,b,c;
		cin >> a >> b >> c;
		if (a == -2) break;
		if (a == -1) {
			deck.draw(&hand);
			field.can_retreat = false;
			field.attached = false;
			field.supporter = false;
			for (int i = 0; i < 6; i++) field.evolved[i] = false;
			cout << "Did you take a prize? -1 if not, ID if you did" << endl;
			int takePrize; cin >> takePrize;
			if (takePrize != -1) {
				hand.add_from_void(takePrize);
			}
			cout << "How much damage did you take?" << endl;
			int taken; cin >> taken;
			field.active->damage += taken;
			if (field.active->damage >= field.active->HP) {
				if (field.benchsize == 0) {
					cout << "You lose!" << endl;
					break;
				}
				else {
					int promote = 5;
					bool not_stop = true;
					while (not_stop) {
						cout << "Pokemon knocked out. Which slot to promote?" << endl;
						cin >> promote;
						if (field.bench[promote] != NULL) {
							field.promote(promote);
							not_stop = false;
						}
					} 
				}
			}
		}
		else if (!hand.play(a,&field,&deck,b,c)) cout << "Error, you can't do that move..." << endl;
	}

	//just delete stuff, no memory leaks...

	delete field.active;
 	for (int i = 0; i < 5; i++) delete field.bench[i];

}


void play_a_random_game() { //here, you actually play a random game controlled by this program


	Deck deck = Deck();
	Hand hand = Hand();
	Field field = Field();
	deck.shuffle();
	set_prizes(&deck,&field);
	
	while (hand.number[1] == 0 && hand.number[4] == 0) {
		hand.shuffleHand(&deck);
		for (int i = 0; i < 7; i++) deck.draw(&hand);
	}

	display(&deck,&hand,&field);


	cout << "Choose your active Pokemon:" << endl;
	int activenum = 0;
	while ((activenum != 1 && activenum != 4) || hand.number[activenum] == 0) {
		cin >> activenum;
	}

	field.place(activenum,5);
	hand.number[activenum]--;

	int total_damage = 0;

	while(true) {

		display(&deck,&hand,&field);

		int a,b,c;
		cin >> a >> b >> c;
		if (a == -2) break;
		if (a == -1) {
			deck.draw(&hand);
			field.can_retreat = false;
			field.attached = false;
			field.supporter = false;
			for (int i = 0; i < 6; i++) field.evolved[i] = false;
			cout << "Did you take a prize? -1 if not, ID if you did" << endl;
			int takePrize; cin >> takePrize;
			if (takePrize != -1) {
				hand.add_from_void(takePrize);
			}
			cout << "How much damage did you take?" << endl;
			int taken; cin >> taken;
			field.active->damage += taken;
			if (field.active->damage >= field.active->HP) {
				if (field.benchsize == 0) {
					cout << "You lose!" << endl;
					break;
				}
				else {
					int promote = 5;
					bool not_stop = true;
					while (not_stop) {
						cout << "Pokemon knocked out. Which slot to promote?" << endl;
						cin >> promote;
						if (field.bench[promote] != NULL) {
							field.promote(promote);
							not_stop = false;
						}
					} 
				}
			}
		}
		else if (!hand.play(a,&field,&deck,b,c)) cout << "Error, you can't do that move..." << endl;
	}



	//just delete stuff, don't want no memory leaks

	delete field.active;
 	for (int i = 0; i < 5; i++) delete field.bench[i];

}


int main () {
	Field field = Field();
	Hand hand = Hand();
	Deck deck = Deck();
	deck.shuffle();
	field.place(1,5);

	play_a_game();
	

	return 0;
}