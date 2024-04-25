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
        if (!isMoveLegal(startPos, endPos)) {
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
        int dy = isWhite ? startPos.y() - endPos.y() : endPos.y() - startPos.y();  // White pawns move up, black pawns move down
        return (dx == 0 && (dy == 100 || (dy == 200 && firstMove))) || (dx == 100 && dy == 100);  // Move forward or capture diagonally
    }

}
