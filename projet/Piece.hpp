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

        virtual bool isMoveLegal(const QPointF& startPos, const QPointF& endPos) = 0;
        virtual bool isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) = 0;

        Piece* isPositionOccupieds(int x, int y, QGraphicsScene* scene, const QGraphicsItem* excludeItem = nullptr);
        bool pathIsClear(const QPointF& startPos, const QPointF& endPos);

        Piece* findKing(QGraphicsScene* scene, bool isWhite);
        bool isKingInCheck(QGraphicsScene* scene, bool isWhiteKing);
        
        bool isWhite;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

        Type type;
        QPointF dragStartPos;  // Position initiale pour les mouvements
    };

}

#endif // PIECE_HPP
