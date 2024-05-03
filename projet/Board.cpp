// Board.cpp
#include "Board.hpp"
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

namespace model {

    Board::Board() {}

    QRectF Board::boundingRect() const {
        return QRectF(0, 0, 800, 800);  // taille du plateau
    }

    void Board::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        painter->setBrush(Qt::gray);  // Couleur de fond pour le plateau
        painter->drawRect(boundingRect());  // Dessine le fond du plateau

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if ((row + col) % 2 == 0)
                    painter->setBrush(Qt::white);
                else
                    painter->setBrush(Qt::darkCyan);
                painter->drawRect(100 * col, 100 * row, 100, 100);
            }
        }
    }

    bool Board::isPositionOccupied(int x, int y, QGraphicsScene* scene, const QGraphicsItem* excludeItem) {
        // Convertir les coordonnées en coordonnées de la scène basées sur la taille de chaque case, par exemple 100x100 pixels par case
        QPointF point(x * 100 + 50, y * 100 + 50); // +50 pour cibler le milieu de la case
        QList<QGraphicsItem*> items = scene->items(point);

        //for (QGraphicsItem* item : items) {
        //    if (dynamic_cast<Piece*>(item)) {
        //        return true;
        //    }
        //}
        //return false;

        qDebug() << "Position: (" << x << "," << y << ") - Items count: " << items.count();

        for (QGraphicsItem* item : items) {
            qDebug() << "Item type:" << typeid(*item).name(); // Afficher le type de chaque objet
            if (item != excludeItem && dynamic_cast<Piece*>(item)) {
                return true;
            }
        }
        return false;
    }

}
 