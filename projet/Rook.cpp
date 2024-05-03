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
        if (!isMoveLegal(startPos, endPos) || Board::isPositionOccupied(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this)) {
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

        if (dx != 0 && dy != 0) {
            return false;  // Le mouvement doit être horizontal ou vertical.
        }

        int stepX = (dx == 0) ? 0 : ((endPos.x() > startPos.x()) ? 100 : -100);
        int stepY = (dy == 0) ? 0 : ((endPos.y() > startPos.y()) ? 100 : -100);

        qreal nextX = startPos.x() + stepX;
        qreal nextY = startPos.y() + stepY;

        while (nextX != endPos.x() || nextY != endPos.y()) {
            if (Board::isPositionOccupied(qRound(nextX / 100), qRound(nextY / 100), scene(), this)) {
                return false;  // Blocage trouvé, mouvement illégal
            }
            nextX += stepX;
            nextY += stepY;
        }
        return true;
    }

}
