#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

namespace model {

    class Knight : public Piece {
    public:
        Knight(bool isWhite);
        ~Knight() override;

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        bool isMoveLegal(const QPointF& startPos, const QPointF& endPos);

    private:
        QPointF startPos;
        QPixmap pixmap;
    };

}

#endif // KNIGHT_HPP
