#include "core/board.h"
#include "core/game.h"
#include "core/tile.h"

#include "gui/qtile.h"
#include "gui/qresetbutton.h"
#include "gui/qgameboard.h"


#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QString>
#include <QDebug>



QGameBoard::~QGameBoard(){
    delete game;
}

QGameBoard::QGameBoard(QWidget *parent) : QWidget(parent){
    resize(650, 450);

    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    boardLayout = NULL;

    game = new Game(4);
    game->registerObserver(this);

    gui_board.resize(game->getGameBoard()->getDimension());
    for(int i = 0; i < game->getGameBoard()->getDimension(); ++i){
        gui_board[i].resize(game->getGameBoard()->getDimension());
    }
    for(int i = 0; i < game->getGameBoard()->getDimension(); ++i){
        for(int j = 0; j < game->getGameBoard()->getDimension(); ++j){
            gui_board[i][j] = NULL;
        }
    }
    drawBoard();


    score = new QLabel(QString("SCORE: %1").arg(game->getScore()));
    score->setStyleSheet("QLabel1 { color: rgb(235, 224, 214); font: 16pt; }");
    score->setFixedHeight(50);
    mainLayout->insertWidget(1, score, 0, Qt::AlignRight);

    setStyleSheet("QGameBoard { background-color: rgb(187, 173, 160) }");

    connect(gameOverWindow.getResetBtn(), SIGNAL(clicked()), this, SLOT(resetGame()));
}

void QGameBoard::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        game->move(UP);
        break;

    case Qt::Key_Left:
        game->move(LEFT);
        break;

    case Qt::Key_Right:
        game->move(RIGHT);
        break;

    case Qt::Key_Down:
        game->move(DOWN);
        break;
    }
}

void QGameBoard::notify(){
    if(game->isGameOver()){
        gameOverWindow.show();
    }

    if(game->won()){
        score->setText(QString("You hit 2048, congratulations! Keep playing to increase your score.\t\t SCORE: %1").arg(game->getScore()));
    }
    else{
        score->setText(QString("SCORE: %1").arg(game->getScore()));
    }

    drawBoard();
}

void QGameBoard::drawBoard(){
    delete boardLayout;

    boardLayout = new QGridLayout();
    for(int i = 0; i < game->getGameBoard()->getDimension(); ++i){
        for(int j = 0; j < game->getGameBoard()->getDimension(); ++j){
            delete gui_board[i][j];

            gui_board[i][j] = new QTile(game->getGameBoard()->getTile(i, j));
            boardLayout->addWidget(gui_board[i][j], i, j);
            gui_board[i][j]->draw();
        }
    }

    mainLayout->insertLayout(0, boardLayout);
}

void QGameBoard::resetGame(){
    game->restart();
    drawBoard();
    score->setText(QString("SCORE: %1").arg(game->getScore()));
    gameOverWindow.hide();
}
