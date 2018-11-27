#pragma once
#include <string>
#include "Board.h"
using namespace std;


/**
* A player plays the game of Tic Tac Toe
* against another player
*
* @author Carlos Hernandez Rosas
*
*/
#ifndef PLAYER_H
#define PLAYER_H
class Player {
private:

	char mark;

	Player* opponent;

	Board board;

public:
	Player();
	Player(char m);

	void setOpponent(Player *op);

	void setBoard(Board b);

	void makeMove();

	void play();

	bool isValid(int row, int col);

	void selection(int choice, int &row, int &col);

	static void makeSelection(int choice);

};
#endif
