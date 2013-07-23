#include "board_layout.h"

const int kScoreColumn = MAX_SIZE + 2;
const int kScoreLine = BOARD_SIZE - 1;
const int kShowItemColumn = MAX_SIZE + 2;
const int kStartButtonLine = 1;
const int kBlackCountTextLine = 2;
const int kBlackCountLcdLine = 3;
const int kWhiteCountTextLine = 4;
const int kWhiteCountLcdLine = 5;

/**
  * BoardLayout class constructor.
  * \param game A pointer to the game environment.
  * \param parent Parent widget.
  **/
BoardLayout::BoardLayout(OthelloGame *const& game, QWidget * parent) : QGridLayout(parent), game_(game)
{
    black_ = QPixmap(BLACK_CELL);
    white_ = QPixmap(WHITE_CELL);
    empty_ = QPixmap(EMPTY_CELL);
    allowed_ = QPixmap(ALLOWED_CELL);
    blackCountLcd_ = new QLCDNumber();
    whiteCountLcd_ = new QLCDNumber();
}

/**
  * Initiate a graphic othello grid, with special spaces for score and start button.
  * If the grid already exists, only reconfigure it into first state.
  * \param first Boolean false by default, set to true if this is the first initialization.
  **/
void BoardLayout::initiate(bool first) {
    Cell *label = NULL;
    int midlow = (BOARD_SIZE / 2);
    int midhigh = (BOARD_SIZE / 2) + 1;
    this->setHorizontalSpacing(0);
    this->setVerticalSpacing(0);
    for(int i = 1; i <= BOARD_SIZE; i++) {
        for(int j = 1; j <= BOARD_SIZE; j++) {
            if(first) {
                label = new Cell(i, j, this);
                label->setMinimumSize(50,50);                
                label->setMaximumSize(50,50);
                addWidget(label, i, j);
            } else {
                label = (Cell*)itemAtPosition(i, j)->widget();
            }
            if(i == j && (i == midhigh || i == midlow))
                label->setPixmap(black_);
            else if ((i == midhigh && j == midlow) || (i == midlow && j == midhigh))
                label->setPixmap(white_);
            else
                label->setPixmap(empty_);
        }
    }
    if(first) {
        addWidget(new StartButton(this), kStartButtonLine, kShowItemColumn);
        addWidget(new QLabel("Black counts:"), kBlackCountTextLine, kShowItemColumn);
        addWidget(blackCountLcd_, kBlackCountLcdLine, kShowItemColumn);
        addWidget(new QLabel("White counts:"), kWhiteCountTextLine, kShowItemColumn);
        addWidget(whiteCountLcd_, kWhiteCountLcdLine, kShowItemColumn);
        QLabel * score = new QLabel();
        score->setMargin(10);
        addWidget(score, kScoreLine, kScoreColumn);
    } else {
        QLabel * score = (QLabel *) itemAtPosition(kScoreLine, kScoreColumn)->widget();
        score->hide();
    }
}

/**
  * Change the type of the cell at position (i,j) to \code{type}.
  * \param i Line position.
  * \param j Column position.
  * \param type New type of the cell (black or white).
  **/
void BoardLayout::changeCellType(int i, int j, CellType type) {
    QLabel *cell = (QLabel *) itemAtPosition(i, j)->widget();
    if (BLACK == type)
        cell->setPixmap(black_);
    else if (WHITE == type)
        cell->setPixmap(white_);
}

/**
  * Set the cell at position (i,j) as clickable.
  * \param i Line position.
  * \param j Column position.
  **/
void BoardLayout::setCellClickable(int i, int j) {
    Cell *cell = (Cell*) itemAtPosition(i, j)->widget();
    cell->setClickable(true);
}

/**
  * Set all cells unclickable and change their image to "empty".
  **/
void BoardLayout::setUnclickable() {
    for(int i = 1; i <= BOARD_SIZE; i++) {
        for(int j = 1; j <= BOARD_SIZE; j++) {
            Cell *cell = (Cell*) itemAtPosition(i, j)->widget();
            if(cell->isClickable()) {
                cell->setClickable(false);
                cell->setPixmap(empty_);
            }
        }
    }
}

/**
  * Set all the cells which hold a playable move with the "allowed" image.
  **/
void BoardLayout::setAllowed() {
    for(int i = 1; i <= BOARD_SIZE; i++) {
        for(int j = 1; j <= BOARD_SIZE; j++) {
            Cell * cell = (Cell *) itemAtPosition(i, j)->widget();
            if(cell->isClickable())
                cell->setPixmap(allowed_);
        }
    }
}

/**
  * Compute and show the winner.
  * \param score The difference between the number of black discs and the number of white ones.
  **/
void BoardLayout::win(int score) {
    QLabel * _score = (QLabel *) itemAtPosition(kScoreLine, kScoreColumn)->widget();
    if(score == 0) {
        _score->setText("Draw.");
    } else if (score < 0) {
        _score->setText("White wins !");
    } else {
        _score->setText("Black wins !");
    }
    _score->show();
}
