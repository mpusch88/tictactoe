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
	Player* currentPlayer;
	Board* board;
	Board b;
public:
	Game();

	void setGame();

	void updateBoard(int spot);

	void updatePlayer();

	void reset();

};