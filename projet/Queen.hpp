#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Piece.hpp"

namespace model {

    class Queen : public Piece {
    public:
        Queen(bool isWhite);
        ~Queen() override;

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        bool isMoveLegal(const QPointF& startPos, const QPointF& endPos) override;
        bool isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) override;

    private:
        QPointF startPos;
        QPixmap pixmap;
    };

}

#endif // QUEEN_HPP
