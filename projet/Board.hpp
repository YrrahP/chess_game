// Board.hpp
#ifndef BOARD_HPP
#define BOARD_HPP

#include <QGraphicsItem>
#include "Piece.hpp"

namespace model {

    class Board : public QGraphicsItem {
    public:
        Board();

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
        static bool isPositionOccupied(int x, int y, QGraphicsScene* scene, const QGraphicsItem* excludeItem = nullptr);
    };

}

#endif // BOARD_HPP
