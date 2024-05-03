#include "Pawn.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    Pawn::Pawn(bool isWhite) : Piece(Type::Pawn, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        pixmap = QPixmap(isWhite ? ":/chesspieces/pictures/pawn_white.png" : ":/chesspieces/pictures/pawn_black.png");
    }

    Pawn::~Pawn() {}

    void Pawn::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            painter->setBrush(isWhite ? Qt::white : Qt::black);
            painter->drawRect(boundingRect());
        }
    }

    void Pawn::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        Piece::mousePressEvent(event);
    }

    void Pawn::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos) || Board::isPositionOccupied(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this)) {
            setPos(startPos);  // Revert to the original position if the move is not legal
        }
        else {
            setPos(endPos);
            firstMove = false;  // The pawn has moved and cannot move two spaces anymore
        }
        Piece::mouseReleaseEvent(event);
    }

    bool Pawn::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        int dx = std::abs(endPos.x() - startPos.x());
        int forward = (isWhite ? -100 : 100);  // White moves up, Black moves down.
        int startRow = isWhite ? 6 : 1;  // Starting rows for White and Black pawns.

        // Avance d'une ou deux cases
        if (dx == 0 && (endPos.y() == startPos.y() + forward || (firstMove && endPos.y() == startPos.y() + 2 * forward))) {
            qreal nextY = startPos.y() + forward;
            while (nextY != endPos.y()) {
                if (Board::isPositionOccupied(qRound(startPos.x() / 100), qRound(nextY / 100), scene(), this)) {
                    return false;
                }
                nextY += forward;
            }
            return true;
        }

        // Capture diagonale
        if (dx == 100 && std::abs(endPos.y() - startPos.y()) == 100) {
            return Board::isPositionOccupied(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), nullptr);
        }

        return false;
    }

}
