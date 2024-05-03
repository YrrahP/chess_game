// King.cpp
#include "King.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    King::King(bool isWhite) : Piece(Type::King, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        if (numKing_ < 2)
        {
            numKing_++;
        }
        else
            throw Over2King("More than 2 Kings :(");

        if (isWhite) {
            pixmap = QPixmap(":/chesspieces/pictures/king_white.png");
        }
        else {
            pixmap = QPixmap(":/chesspieces/pictures/king_black.png");
        }
    }

    King::~King() 
    {
        numKing_--;
    }

    void King::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            // fallback if the image fails to load
            painter->setBrush(isWhite ? Qt::yellow : Qt::black);
            painter->drawEllipse(30, 20, 40, 60);
        }
    }

    void King::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        QGraphicsItem::mousePressEvent(event);
    }

    void King::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QGraphicsItem::mouseReleaseEvent(event);

        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos) || Board::isPositionOccupied(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this)) {
            setPos(startPos);
        }
        else {
            setPos(endPos);
        }
    }

    bool King::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        qreal dx = std::abs(endPos.x() - startPos.x());
        qreal dy = std::abs(endPos.y() - startPos.y());
        return (dx <= 100 && dy <= 100 && (dx > 0 || dy > 0));
    }

}
