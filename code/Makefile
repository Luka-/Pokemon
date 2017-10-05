INCLUDES = -Ilib/sqlite
main : main.o hand.o deck.o field.o pokemon.o
	g++ -g main.o hand.o deck.o field.o pokemon.o -o main

main.o : hand.h deck.h main.cpp
	g++ -c main.cpp

deck.o : deck.cpp deck.h hand.h
	g++ -c deck.cpp

hand.o : hand.cpp hand.h deck.h
	g++ -c hand.cpp

field.o : field.cpp field.h deck.h hand.h pokemon.h
	g++ -c field.cpp

pokemon.o : pokemon.cpp pokemon.h
	g++ -c pokemon.cpp
