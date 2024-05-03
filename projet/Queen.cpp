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
        if (!isMoveLegal(startPos, endPos) || Board::isPositionOccupied(qRound(endPos.x() / 100), qRound(endPos.y() / 100), scene(), this)) {
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

        if (!(dx == 0 || dy == 0 || dx == dy)) {  // La reine se déplace en ligne droite ou diagonalement
            return false;
        }

        int stepX = (endPos.x() > startPos.x()) ? 100 : (endPos.x() < startPos.x()) ? -100 : 0;
        int stepY = (endPos.y() > startPos.y()) ? 100 : (endPos.y() < startPos.y()) ? -100 : 0;

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
