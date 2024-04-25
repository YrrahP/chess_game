#include "Bishop.hpp"
#include <QPainter>
#include <QPixmap>
#include <cmath>

namespace model {

    Bishop::Bishop(bool isWhite) : Piece(Type::Bishop, isWhite) {
        setFlag(QGraphicsItem::ItemIsMovable);
        pixmap = QPixmap(isWhite ? ":/chesspieces/pictures/bishop_white.png" : ":/chesspieces/pictures/bishop_black.png");
    }

    Bishop::~Bishop() {}

    void Bishop::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        if (!pixmap.isNull()) {
            painter->drawPixmap(boundingRect().toRect(), pixmap);
        }
        else {
            painter->setBrush(isWhite ? Qt::white : Qt::black);
            painter->drawRect(boundingRect());  // Dessiner un rectangle simple en cas de secours
        }
    }

    void Bishop::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        startPos = this->pos();
        Piece::mousePressEvent(event);
    }

    void Bishop::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
        QPointF endPos = QPointF(qRound(this->pos().x() / 100) * 100, qRound(this->pos().y() / 100) * 100);
        if (!isMoveLegal(startPos, endPos)) {
            setPos(startPos);  // Revenir � la position initiale si le mouvement n'est pas l�gal
        }
        else {
            setPos(endPos);  // Confirmer le nouveau positionnement
        }
        Piece::mouseReleaseEvent(event);
    }

    bool Bishop::isMoveLegal(const QPointF& startPos, const QPointF& endPos) {
        int dx = std::abs(endPos.x() - startPos.x());
        int dy = std::abs(endPos.y() - startPos.y());
        return dx == dy;  // Mouvements l�gaux uniquement en diagonale
    }

}
