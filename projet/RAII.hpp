#ifndef RAII_HPP
#define RAII_HPP

#include <QPointF>
#include "Piece.hpp"

namespace model {

    class RAII {
    public:
        RAII(Piece& piece, const QPointF& newPosition);
        ~RAII();

    private:
        Piece& piece;
        QPointF originalPosition;
    };

}

#endif // RAII_HPP
