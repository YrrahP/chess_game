#include "Rook.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    Rook::Rook(bool isWhite) : Piece(Type::Rook, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        pixmap = QPixmap(isWhite ? ":/chesspieces/pictures/rook_white.png" : ":/chesspieces/pictures/rook_black.png");
    }

    Rook::~Rook() {}

    void Rook::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            painter->setBrush(isWhite ? Qt::white : Qt::black);
            painter->drawRect(boundingRect());  // Draw a simple rectangle as fallback
        }
    }

    void Rook::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        Piece::mousePressEvent(event);
    }

    void Rook::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos)) {
            setPos(startPos);  // Revert to the original position if the move is not legal
        }
        else {
            setPos(endPos);  // Confirm the new position
        }
        Piece::mouseReleaseEvent(event);
    }

    bool Rook::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        qreal dx = std::abs(endPos.x() - startPos.x());
        qreal dy = std::abs(endPos.y() - startPos.y());
        return (dx == 0 || dy == 0);  // Legal move if in a straight line horizontally or vertically
    }

}
