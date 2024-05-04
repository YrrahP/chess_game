#ifndef PAWN_HPP
#define PAWN_HPP

#include "Piece.hpp"

namespace model {

    class Pawn : public Piece {
    public:
        Pawn(bool isWhite);
        ~Pawn() override;

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        bool isMoveLegal(const QPointF& startPos, const QPointF& endPos) override;
        bool isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) override;

    private:
        QPointF startPos;
        QPixmap pixmap;
        bool firstMove = true;  // Used to determine if the pawn can move two spaces
    };

}

#endif // PAWN_HPP
