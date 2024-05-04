#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "Piece.hpp"

namespace model {

    class Bishop : public Piece {
    public:
        Bishop(bool isWhite);
        ~Bishop() override;

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

#endif // BISHOP_HPP
