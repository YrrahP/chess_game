#include "Knight.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    Knight::Knight(bool isWhite) : Piece(Type::Knight, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        pixmap = QPixmap(isWhite ? ":/chesspieces/pictures/knight_white.png" : ":/chesspieces/pictures/knight_black.png");
    }

    Knight::~Knight() {}

    void Knight::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            painter->setBrush(isWhite ? Qt::white : Qt::black);
            painter->drawRect(boundingRect());  // Dessiner un rectangle simple en cas de secours
        }
    }

    void Knight::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        Piece::mousePressEvent(event);
    }

    void Knight::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos)) {
            setPos(startPos);  // Revenir à la position initiale si le mouvement n'est pas légal
        }
        else {
            setPos(endPos);  // Confirmer le nouveau positionnement
        }
        Piece::mouseReleaseEvent(event);
    }

    bool Knight::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        int dx = std::abs(endPos.x() - startPos.x());
        int dy = std::abs(endPos.y() - startPos.y());
        return (dx == 200 && dy == 100) || (dx == 100 && dy == 200);  // Mouvements en "L"
    }

}
