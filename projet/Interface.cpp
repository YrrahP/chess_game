// Interface.cpp
#include "Interface.hpp"
#include "King.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Pawn.hpp"

namespace interface {

    Interface::Interface(QGraphicsScene* scene) : scene(scene) {}

    void Interface::drawBoard() {
        model::Board* board = new model::Board();
        scene->addItem(board);
    }

    void Interface::drawStartButton() {
        QPushButton* startButton = new QPushButton("Reset");
        QGraphicsProxyWidget* proxyWidget = scene->addWidget(startButton);
        proxyWidget->setPos(1200, 100);
        startButton->setFixedSize(200, 100);
        startButton->setStyleSheet("font-size: 24pt; font-weight: bold; color: white; background-color: darkCyan; ");
        QObject::connect(startButton, &QPushButton::clicked, [&]() {
            resetGame();  // Appelle la méthode de réinitialisation lorsque le bouton est cliqué
            });
    }

    void Interface::resetGame() {
        // Supprime toutes les pièces actuelles de la scène
        foreach(QGraphicsItem * item, scene->items()) {
            model::Piece* piece = dynamic_cast<model::Piece*>(item);
            if (piece) {
                scene->removeItem(piece);
                delete piece;
            }
        }

        // Redessine le plateau et replace les pièces à leur position initiale
        drawBoard();
        setupPieces();
    }

    void Interface::setupPieces() {
        // White pieces
        model::King* whiteKing = new model::King(true);
        model::Queen* whiteQueen = new model::Queen(true);
        model::Rook* whiteRook1 = new model::Rook(true);
        model::Rook* whiteRook2 = new model::Rook(true);
        model::Knight* whiteKnight1 = new model::Knight(true);
        model::Knight* whiteKnight2 = new model::Knight(true);
        model::Bishop* whiteBishop1 = new model::Bishop(true);
        model::Bishop* whiteBishop2 = new model::Bishop(true);
        // Black Pieces
        model::King* blackKing = new model::King(false);
        model::Queen* blackQueen = new model::Queen(false);
        model::Rook* blackRook1 = new model::Rook(false);
        model::Rook* blackRook2 = new model::Rook(false);
        model::Knight* blackKnight1 = new model::Knight(false);
        model::Knight* blackKnight2 = new model::Knight(false);
        model::Bishop* blackBishop1 = new model::Bishop(false);
        model::Bishop* blackBishop2 = new model::Bishop(false);

        // White Pieces
        whiteKing->setPos(400, 700);
        whiteQueen->setPos(300, 700);
        whiteRook1->setPos(0, 700);
        whiteRook2->setPos(700, 700);
        whiteKnight1->setPos(100, 700);
        whiteKnight2->setPos(600, 700);
        whiteBishop1->setPos(200, 700);
        whiteBishop2->setPos(500, 700);
        // Black Pieces
        blackKing->setPos(400, 0);
        blackQueen->setPos(300, 0);
        blackRook1->setPos(0, 0);
        blackRook2->setPos(700, 0);
        blackKnight1->setPos(100, 0);
        blackKnight2->setPos(600, 0);
        blackBishop1->setPos(200, 0);
        blackBishop2->setPos(500, 0);

        // White Pieces
        scene->addItem(whiteKing);
        scene->addItem(whiteQueen);
        scene->addItem(whiteRook1);
        scene->addItem(whiteRook2);
        scene->addItem(whiteKnight1);
        scene->addItem(whiteKnight2);
        scene->addItem(whiteBishop1);
        scene->addItem(whiteBishop2);
        // Black Pieces
        scene->addItem(blackKing);
        scene->addItem(blackQueen);
        scene->addItem(blackRook1);
        scene->addItem(blackRook2);
        scene->addItem(blackKnight1);
        scene->addItem(blackKnight2);
        scene->addItem(blackBishop1);
        scene->addItem(blackBishop2);

        // Pawns
        for (int i = 0; i < 8; i++) {
            model::Pawn* whitePawn = new model::Pawn(true);
            model::Pawn* blackPawn = new model::Pawn(false);
            whitePawn->setPos(100 * i, 600);  // Position initiale des pions blancs
            blackPawn->setPos(100 * i, 100);  // Position initiale des pions noirs
            scene->addItem(whitePawn);
            scene->addItem(blackPawn);
        }
    }

}
