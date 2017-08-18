#include "core/game.h"
#include "core/tile.h"


Game::Game(int dimension){
    score = 0;
    board = new Board(dimension);
    restart();
}


Game::~Game(){
    delete board;
}


void Game::move(Direction dir){
    board->move(dir);

    if(board->isTileCollisionLastRound()){
        score += board->getPointsScoredLastRound();
    }

    if(!board->movePossible()){
        gameOver = true;
    }

    notifyObservers();
}


void Game::restart(){
    board->reset();
    gameOver = false;
    score = 0;
}


bool Game::won() const{
    for(int i = 0; i < board->getDimension(); ++i){
        for(int j = 0; j < board->getDimension(); ++j){
            if(board->getTile(i, j) != NULL && board->getTile(i, j)->getValue() == WINNING_VALUE){
                return true;
            }
        }
    }

    return false;
}

