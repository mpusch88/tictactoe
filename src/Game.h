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
	Player *winner;
	Board* board;
	Board b;
	bool gameOver;
	
public:
	Game();
	Player* getPlayer();
	void updateBoard(int spot);
	void updatePlayer();
	void reset();
	bool getStatus();
	Board getBoard();
	void setWinner();
	Player* getWinner();
};