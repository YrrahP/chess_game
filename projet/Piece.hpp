// Piece.hpp
#ifndef PIECE_HPP
#define PIECE_HPP

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "Board.hpp"

namespace model {

    class Piece : public QGraphicsItem {
    public:
        enum class Type { King, Queen, Rook, Bishop, Knight, Pawn };
        Piece(Type type, bool isWhite);

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

        Piece* isPositionOccupieds(int x, int y, QGraphicsScene* scene, const QGraphicsItem* excludeItem = nullptr);
        bool pathIsClear(const QPointF& startPos, const QPointF& endPos);
        
        bool isWhite;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

        Type type;
        QPointF dragStartPos;  // Position initiale pour les mouvements
    };

}

#endif // PIECE_HPP
