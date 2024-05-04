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
        int dx = std::abs(endPos.x() - startPos.x());
        int dy = std::abs(endPos.y() - startPos.y());

        // Mouvement horizontal ou vertical
        if ((dx == 0 || dy == 0) && pathIsClear(startPos, endPos)) {
            // V�rifier si la position de destination est occup�e par une pi�ce adverse
            Piece* targetPiece = isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this);
            if (targetPiece && targetPiece->isWhite != this->isWhite) {
                // Si une pi�ce adverse est pr�sente, elle est retir�e de l'�chiquier
                scene()->removeItem(targetPiece);
                delete targetPiece;  // Lib�rer la m�moire de la pi�ce captur�e
                return true;
            }
            return targetPiece == nullptr;  // Si aucune pi�ce n'est pr�sente, le mouvement est l�gal
        }
        return false;
    }

    bool Rook::isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) {
        int dx = std::abs(endPos.x() - startPos.x());
        int dy = std::abs(endPos.y() - startPos.y());
        return (dx == 0 || dy == 0) && pathIsClear(startPos, endPos);  // Mouvement horizontal ou vertical
    }


}
