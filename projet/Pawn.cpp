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
        int dy = endPos.y() - startPos.y();
        int forward = (isWhite ? -100 : 100);  // White moves up, Black moves down.

        // Avance d'une case
        if (dx == 0 && dy == forward) {
            if (isPositionOccupieds(qRound(startPos.x() / 100), qRound(endPos.y() / 100), scene(), this) == nullptr) {
                return true;
            }
        }

        // Avance de deux cases depuis la position initiale
        if (dx == 0 && std::abs(dy) == 200 && (isWhite ? startPos.y() == 600 : startPos.y() == 100)) {
            if (isPositionOccupieds(qRound(startPos.x() / 100), qRound(startPos.y() / 100 + forward), scene(), this) == nullptr &&
                isPositionOccupieds(qRound(startPos.x() / 100), qRound(endPos.y() / 100), scene(), this) == nullptr) {
                return true;
            }
        }

        // Capture diagonale
        if (dx == 100 && std::abs(dy) == 100) {
            // Ajouter une vérification pour s'assurer que le mouvement est vers l'avant selon la couleur de la pièce
            if ((isWhite && dy == -100) || (!isWhite && dy == 100)) {
                Piece* targetPiece = isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this);
                if (targetPiece != nullptr && targetPiece->isWhite != this->isWhite) {
                    scene()->removeItem(targetPiece);
                    delete targetPiece;
                    return true;
                }
            }
        }

        return false;
    }

    bool Pawn::isMoveLegalForCheck(const QPointF& startPos, const QPointF& endPos) {
        int dx = endPos.x() - startPos.x();
        int dy = endPos.y() - startPos.y();
        if (this->isWhite) {
            // Avancer d'une case vers le haut
            if (dx == 0 && dy == -100 && !isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene())) {
                return true;
            }
            // Avancer de deux cases vers le haut sur le premier coup
            if (this->firstMove && dx == 0 && dy == -200 &&
                !isPositionOccupieds(qRound(endPos.x() / 100), qRound((endPos.y() + 100) / 100), scene()) &&
                !isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene())) {
                return true;
            }
            // Capture diagonale
            if ((dx == 100 || dx == -100) && dy == -100) {
                Piece* targetPiece = isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene());
                if (targetPiece && targetPiece->isWhite != this->isWhite) {
                    return true;
                }
            }
        }
        else {
            // Avancer d'une case vers le bas
            if (dx == 0 && dy == 100 && !isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene())) {
                return true;
            }
            // Avancer de deux cases vers le bas sur le premier coup
            if (this->firstMove && dx == 0 && dy == 200 &&
                !isPositionOccupieds(qRound(endPos.x() / 100), qRound((endPos.y() - 100) / 100), scene()) &&
                !isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene())) {
                return true;
            }
            // Capture diagonale
            if ((dx == 100 || dx == -100) && dy == 100) {
                Piece* targetPiece = isPositionOccupieds(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene());
                if (targetPiece && targetPiece->isWhite != this->isWhite) {
                    return true;
                }
            }
        }
        return false;
    }


}
