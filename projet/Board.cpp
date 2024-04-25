// Board.cpp
#include "Board.hpp"
#include <QPainter>

namespace model {

    Board::Board() {}

    QRectF Board::boundingRect() const {
        return QRectF(0, 0, 800, 800);  // taille du plateau
    }

    void Board::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        painter->setBrush(Qt::gray);  // Couleur de fond pour le plateau
        painter->drawRect(boundingRect());  // Dessine le fond du plateau

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if ((row + col) % 2 == 0)
                    painter->setBrush(Qt::white);
                else
                    painter->setBrush(Qt::darkCyan);
                painter->drawRect(100 * col, 100 * row, 100, 100);
            }
        }
    }

}
