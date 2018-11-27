#include "Game.h"

Game::Game(){
   p1 = Player('X');
   p2 = Player('O');
   board = Board();
}

void Game::runGame() {
	p1.setOpponent(&p2);
	p2.setOpponent(&p1);
	p1.play();
}

void Game::updateBoard(int spot) {
	Player::makeSelection(spot);
}
