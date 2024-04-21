// King.cpp
#include "King.hpp"
#include <QPainter>
#include <cmath>  // Pour std::abs

namespace model {

    King::King(bool isWhite) : Piece(Type::King, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
    }

    void King::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        Piece::paint(painter, option, widget);
        painter->setBrush(isWhite ? Qt::white : Qt::black);
        painter->drawEllipse(30, 20, 40, 60);
    }

    void King::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        QGraphicsItem::mousePressEvent(event);
    }

    void King::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QGraphicsItem::mouseReleaseEvent(event);

        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos)) {
            setPos(startPos);  // Revenir à la position initiale si le mouvement n'est pas légal
        }
        else {
            setPos(endPos);  // Confirmer le nouveau positionnement
        }
    }

    bool King::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        qreal dx = std::abs(endPos.x() - startPos.x());
        qreal dy = std::abs(endPos.y() - startPos.y());
        return (dx <= 100 && dy <= 100 && (dx > 0 || dy > 0));  // Le roi peut se déplacer d'une seule case dans toutes les directions
    }

}
