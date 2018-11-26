#include <string>
#include "Player.h"
#include "Board.h"

using namespace std;

/**
* Constructs a Player with a name and a mark
* @param n name of the player
* @param m mark of the player
*/
Player::Player(char m) {
	mark = m;
}

/**
* Sets up the opponent of the player
* @param op opponent
*/
void Player::setOpponent(Player* op) {
	opponent = op;
}

/**
* Sets up the board the player will play on
* @param b board
*/
void Player::setBoard(Board b) {
	board = b;
}

/**
* Prompt the player to enter the row and column to place his mark.
* If the player chooses and invalid spot, already taken by a mark
* or just not in the table, the player will be prompted to enter
* a valid row and column
*/
void Player::makeMove() {
	int row, col;
	do {
		//TODO Receive the selection 
	} while (!isValid(row, col));
	board.addMark(row, col, mark);
}

/**
* Starts the game, and will continue it until a player has won,
* or the board has been filled. It will then announce the outcome
* of the game.
*/
void Player::play() {
	while (!board.xWins() && !board.oWins() && !board.isFull()) {
		makeMove();
		if (board.xWins() || board.isFull()) {
			break;
		}
		opponent->makeMove();
	}
	if (board.xWins()) {
		// This player wins
	}
	else if (board.oWins()) {
		// opponent wins
	}
	else {
		// draw
	}
}

/**
* Checks if the spot for the given row and column is valid.
*
* @param row the row the mark wants to be placed
* @param col the column the mark wants to be placed
* @return true if the spot is valid, false otherwise
*/
bool Player::isValid(int row, int col) {
	if (row >= 0 && row <= 2 && col >= 0 && col <= 2 && board.getMark(row, col) == ' ') {
		return true;
	}
	return false;
}
