// Interface.cpp
#include "Interface.hpp"
#include "King.hpp"  // Inclure la définition de King

namespace Interface {

    Interface::Interface(QGraphicsScene* scene) : scene(scene) {}

    void Interface::drawBoard() {
        Model::Board* board = new Model::Board();
        scene->addItem(board);
    }

    void Interface::setupPieces() {
        // Créer un roi blanc et un roi noir
        Model::King* whiteKing = new Model::King(true);
        Model::King* blackKing = new Model::King(false);

        // Positionnement des rois
        whiteKing->setPos(400, 0);
        blackKing->setPos(400, 700);

        scene->addItem(whiteKing);
        scene->addItem(blackKing);
    }

}
