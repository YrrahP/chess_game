#include "RAII.hpp"

namespace model {

    RAII::RAII(Piece& piece, const QPointF& newPosition)
        : piece(piece), originalPosition(piece.pos()) {
        piece.setPos(newPosition);
    }

    RAII::~RAII() {
        piece.setPos(originalPosition);
    }

}
