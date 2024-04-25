#include "Queen.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    Queen::Queen(bool isWhite) : Piece(Type::Queen, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        pixmap = QPixmap(isWhite ? ":/chesspieces/pictures/queen_white.png" : ":/chesspieces/pictures/queen_black.png");
    }

    Queen::~Queen() {}

    void Queen::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            painter->setBrush(isWhite ? Qt::white : Qt::black);
            painter->drawEllipse(boundingRect());
        }
    }

    void Queen::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        Piece::mousePressEvent(event);
    }

    void Queen::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos)) {
            setPos(startPos);  // Revenir à la position initiale si le mouvement n'est pas légal
        }
        else {
            setPos(endPos);  // Confirmer le nouveau positionnement
        }
        Piece::mouseReleaseEvent(event);
    }

    bool Queen::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        qreal dx = std::abs(endPos.x() - startPos.x());
        qreal dy = std::abs(endPos.y() - startPos.y());
        return (dx == 0 || dy == 0 || dx == dy);  // Mouvement légal si en ligne droite ou diagonale
    }

}
