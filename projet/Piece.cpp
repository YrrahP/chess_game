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

    Piece* Piece::isPositionOccupieds(int x, int y, QGraphicsScene* scene, const QGraphicsItem* excludeItem) {
        QPointF point(x * 100 + 50, y * 100 + 50); // +50 pour cibler le milieu de la case
        QList<QGraphicsItem*> items = scene->items(point);

        qDebug() << "Position: (" << x << "," << y << ") - Items count: " << items.count();

        for (QGraphicsItem* item : items) {
            qDebug() << "Item type:" << typeid(*item).name(); // Afficher le type de chaque objet
            Piece* piece = dynamic_cast<Piece*>(item);
            if (piece != nullptr && item != excludeItem) {
                return piece;
            }
        }
        return nullptr;
    }

    bool Piece::pathIsClear(const QPointF& startPos, const QPointF& endPos) {
        int xStart = qRound(startPos.x() / 100);
        int yStart = qRound(startPos.y() / 100);
        int xEnd = qRound(endPos.x() / 100);
        int yEnd = qRound(endPos.y() / 100);
        int stepX = (xEnd - xStart != 0) ? (xEnd - xStart) / std::abs(xEnd - xStart) : 0;
        int stepY = (yEnd - yStart != 0) ? (yEnd - yStart) / std::abs(yEnd - yStart) : 0;

        xStart += stepX;  // Commencer à vérifier à partir de la case suivante
        yStart += stepY;

        while (xStart != xEnd || yStart != yEnd) {
            if (isPositionOccupieds(xStart, yStart, scene(), nullptr)) {
                return false;  // Trouvé un obstacle
            }
            xStart += stepX;
            yStart += stepY;
        }

        return true;
    }

    Piece* Piece::findKing(QGraphicsScene* scene, bool isWhite) {
        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            Piece* piece = dynamic_cast<Piece*>(item);
            if (piece && piece->type == Type::King && piece->isWhite == isWhite) {
                return piece;
            }
        }
        return nullptr;
    }

    bool Piece::isKingInCheck(QGraphicsScene* scene, bool isWhiteKing) {
        Piece* king = findKing(scene, isWhiteKing);
        if (!king) return false;

        QList<QGraphicsItem*> items = scene->items();
        for (QGraphicsItem* item : items) {
            Piece* piece = dynamic_cast<Piece*>(item);
            if (piece && piece->isWhite != isWhiteKing) {
                if (piece->isMoveLegalForCheck(piece->pos(), king->pos())) {
                    return true;
                }
            }
        }
        return false;
    }

}
