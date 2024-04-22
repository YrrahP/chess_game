// Interface.cpp
#include "Interface.hpp"
#include "King.hpp"

namespace interface {

    Interface::Interface(QGraphicsScene* scene) : scene(scene) {}

    void Interface::drawBoard() {
        model::Board* board = new model::Board();
        scene->addItem(board);
    }

    void Interface::setupPieces() {
        model::King* whiteKing = new model::King(true);
        model::King* blackKing = new model::King(false);

        whiteKing->setPos(400, 0);
        blackKing->setPos(400, 700);

        scene->addItem(whiteKing);
        scene->addItem(blackKing);
    }

}
