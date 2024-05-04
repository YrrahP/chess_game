#ifndef ROOK_HPP
#define ROOK_HPP

#include "Piece.hpp"

namespace model {

    class Rook : public Piece {
    public:
        Rook(bool isWhite);
        ~Rook() override;

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

#endif // ROOK_HPP
