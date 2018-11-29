#include <iostream>
#include "Player.h"
#include "Board.h"

using namespace std;


Player::Player() {
}
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
void Player::setOpponent(Player &op) {
	opponent = &op;
}

/**
* Sets up the board the player will play on
* @param b board
*/
void Player::setBoard(Board* b) {
	board = b;
}

/**
* Prompt the player to enter the row and column to place his mark.
* If the player chooses and invalid spot, already taken by a mark
* or just not in the table, the player will be prompted to enter
* a valid row and column
*/
void Player::makeMove(int choice) {
	int row, col;
	row = choice / 3;
	col = choice % 3;
	if (isValid(row, col)) {
		board->addMark(row, col, mark);
	}
}

/**
* Starts the game, and will continue it until a player has won,
* or the board has been filled. It will then announce the outcome
* of the game.
*/
void Player::play(int choice) {
	cout << "Started play by '" << mark << "'\n";
	if (!board->xWins() && !board->oWins() && !board->isFull()) {
		makeMove(choice);
	}
	if (board->xWins()) {
		cout << "board at 0 '" << board->getMark(0, 0) << "'\n";
		// This player wins
		cout << "P1 WINS\n";
		
		
	}
	else if (board->oWins()) {
		// opponent wins
		cout << "P2 WINS\n";
		
	}
	else if(board->isFull()){
		// draw
		cout << "DRAW\n";
		
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
	if (row >= 0 && row <= 2 && col >= 0 && col <= 2 && board->getMark(row, col) == ' ') {
		return true;
	}
	return false;
}

void Player::selection(int choice, int &row, int &col) {
	row = choice / 3;
	col = choice % 3;
}

Player* Player::getOpponent() {
	return opponent;
}

char Player::getMark() {
	return mark;
}
