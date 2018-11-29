#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(){
   p1 = Player('X');
   p2 = Player('O');
   b = Board();
   board = &b;
   p1.setOpponent(p2);
   p2.setOpponent(p1);  
   p1.setBoard(board);
   p2.setBoard(board);
   currentPlayer = &p1;
   winner = NULL;
}

void Game::updateBoard(int spot) {
	currentPlayer->play(spot);
	//cout << "mark at 0 = '" << board->getMark(0, 0) << "'";
}

void Game::updatePlayer() {
	currentPlayer = currentPlayer->getOpponent();
}

void Game::reset() {
	board->clear();
}

Player* Game::getPlayer() {
	return currentPlayer;
}

bool Game::getStatus() {
	if (board->xWins() || board->oWins() || board->isFull()) {
		setWinner();
		return true;
	}
	else
		return false;
}

Board Game::getBoard() {
	return b;
}

void Game::setWinner() {
	if (board->xWins()) {
		winner = &p1;
	}
	else if (board->oWins()) {
		winner = &p2;
	}
	else
		winner = NULL;
}

Player* Game::getWinner() {
	return winner;
}