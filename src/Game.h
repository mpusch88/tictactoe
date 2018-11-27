#pragma once
#include "Player.h"

/**
* A Board used to play Tic Tac Toe
*
* @author Carlos Hernandez Rosas
*
*/
class Game {
private:
	Player p1;
	Player p2;
	Board board;
public:
	Game();

	void runGame();

	void updateBoard(int spot);

};