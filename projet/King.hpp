// King.hpp
#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

namespace model {

    class King : public Piece {
    public:
        King(bool isWhite);
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        bool isMoveLegal(const QPointF& startPos, const QPointF& endPos);

    private:
        QPointF startPos;
    };

}

#endif // KING_HPP
