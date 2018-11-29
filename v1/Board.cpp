#include "Board.h"

Board::Board() {
	markCount = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			theBoard[i][j] = ' ';
	}
}

char Board::getMark(int row, int col) {
	return theBoard[row][col];
}

bool Board::isFull() {
	return markCount == 9;
}

bool Board::xWins() {
	if (checkWinner('X') == 1)
		return true;
	else
		return false;
}

bool Board::oWins() {
	if (checkWinner('O') == 1)
		return true;
	else
		return false;
}


void Board::addMark(int row, int col, char mark) {
	theBoard[row][col] = mark;
	markCount++;
}

void Board::clear() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			theBoard[i][j] = ' ';
	markCount = 0;
}

int Board::checkWinner(char mark) {
	int row, col;
	int result = 0;

	for (row = 0; result == 0 && row < 3; row++) {
		int row_result = 1;
		for (col = 0; row_result == 1 && col < 3; col++)
			if (theBoard[row][col] != mark)
				row_result = 0;
		if (row_result != 0)
			result = 1;
	}


	for (col = 0; result == 0 && col < 3; col++) {
		int col_result = 1;
		for (row = 0; col_result != 0 && row < 3; row++)
			if (theBoard[row][col] != mark)
				col_result = 0;
		if (col_result != 0)
			result = 1;
	}
	
	if (result == 0) {
		int diag1Result = 1;
		for (row = 0; diag1Result != 0 && row < 3; row++)
			if (theBoard[row][row] != mark)
				diag1Result = 0;
		if (diag1Result != 0)
			result = 1;
	}
	if (result == 0) {
		int diag2Result = 1;
		for (row = 0; diag2Result != 0 && row < 3; row++)
			if (theBoard[row][3 - 1 - row] != mark)
				diag2Result = 0;
		if (diag2Result != 0)
			result = 1;
	}
	return result;
}