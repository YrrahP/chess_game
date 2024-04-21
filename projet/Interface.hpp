// Interface.hpp
#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <QGraphicsScene>
#include "Board.hpp"

namespace interface {

    class Interface {
    public:
        Interface(QGraphicsScene* scene);
        void drawBoard();
        void setupPieces();

    private:
        QGraphicsScene* scene;
    };

}

#endif // INTERFACE_HPP
