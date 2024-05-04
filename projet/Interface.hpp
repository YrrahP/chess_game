// Interface.hpp
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "Board.hpp"

namespace interface {

    class Interface {
    public:
        Interface(QGraphicsScene* scene);
        void drawBoard();
        void setupPieces();
        void drawStartButton();

    private:
        QGraphicsScene* scene;
    };

}

#endif // INTERFACE_HPP
