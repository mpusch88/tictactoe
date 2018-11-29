#pragma once

/**
* A Board used to play Tic Tac Toe
*
* @author Carlos Hernandez Rosas
*
*/
class Board {
private:
	char theBoard[3][3];
	int markCount;
public:
	Board();

	char getMark(int row, int col);

	bool isFull();

	bool xWins();

	bool oWins();

	
	void addMark(int row, int col, char mark);

	void clear();

	int checkWinner(char mark);

};