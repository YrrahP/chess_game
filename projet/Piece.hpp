// Piece.hpp
#ifndef PIECE_HPP
#define PIECE_HPP

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace model {

    class Piece : public QGraphicsItem {
    public:
        enum class Type { King, Queen, Rook, Bishop, Knight, Pawn };
        Piece(Type type, bool isWhite);

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

        Type type;
        bool isWhite;
        QPointF dragStartPos;  // Position initiale pour les mouvements
    };

}

#endif // PIECE_HPP
