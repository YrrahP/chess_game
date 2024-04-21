// Board.hpp
#ifndef BOARD_HPP
#define BOARD_HPP

#include <QGraphicsItem>

namespace model {

    class Board : public QGraphicsItem {
    public:
        Board();

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    };

}

#endif // BOARD_HPP
