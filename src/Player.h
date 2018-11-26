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
class Player {
private:
	string name;

	char mark;

	Player* opponent;

	Board board;

public:
	Player(string n, char m);

	void setOpponent(Player *op);

	void setBoard(Board b);

	void makeMove();

	void play();

	bool isValid(int row, int col);

};
