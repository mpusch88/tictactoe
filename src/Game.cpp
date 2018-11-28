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
}

void Game::updateBoard(int spot) {
	currentPlayer->play(spot);
	cout << "mark at 0 = '" << board->getMark(0, 0) << "'";
}

void Game::updatePlayer() {
	currentPlayer = currentPlayer->getOpponent();
}

void Game::reset() {
	board->clear();
}