// Piece.cpp
#include "Piece.hpp"

namespace model {

    Piece::Piece(Type type, bool isWhite) : type(type), isWhite(isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    }

    void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        // Sauvegarde la position de départ pour vérifier le mouvement
        dragStartPos = event->pos();
        QGraphicsItem::mousePressEvent(event);
    }

    void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QGraphicsItem::mouseReleaseEvent(event);

        // Alignement sur la grille de 100x100 pixels
        qreal x = qRound(this->pos().x() / 100) * 100;
        qreal y = qRound(this->pos().y() / 100) * 100;
        setPos(x, y);
    }

    QRectF Piece::boundingRect() const {
        return QRectF(0, 0, 100, 100); // Taille standard pour toutes les pièces
    }

    void Piece::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        painter->setBrush(isWhite ? Qt::white : Qt::black);
        painter->drawEllipse(boundingRect());
    }

}
