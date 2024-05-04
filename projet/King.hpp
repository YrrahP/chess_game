// King.hpp
#ifndef KING_HPP
#define KING_HPP

#include <stdexcept>
#include "Piece.hpp"

namespace model {

    class Over2King : public std::exception
    {
    public:
        using std::exception::exception;
    };

    class King : public Piece {
    public:
        King(bool isWhite);
        ~King();
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        bool isMoveLegal(const QPointF& startPos, const QPointF& endPos) override;
        bool isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) override;

    private:
        QPointF startPos;
        QPixmap pixmap;
        static inline int numKing_ = 0;
    };

}

#endif // KING_HPP
