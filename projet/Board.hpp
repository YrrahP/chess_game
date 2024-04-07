#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Rook.hpp"
#include <memory>
#include <vector>
#include <optional>

class Board {
private:
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
    static const int size = 8;  // Taille standard d'un échiquier d'échecs

public:
    Board() {
        // Initialiser la grille de l'échiquier avec des pointeurs nuls
        grid.resize(size, std::vector<std::shared_ptr<Piece>>(size, nullptr));
    }

    void placePiece(const std::shared_ptr<Piece>& piece, int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size) {
            grid[x][y] = piece;
        }
    }

    std::optional<std::shared_ptr<Piece>> getPiece(int x, int y) const {
        if (x >= 0 && x < size && y >= 0 && y < size) {
            return grid[x][y];
        }
        return std::nullopt;
    }

    bool movePiece(int fromX, int fromY, int toX, int toY) {
        auto piece = getPiece(fromX, fromY);
        if (piece && *piece && (*piece)->isValidMove(toX, toY)) {
            // Simuler le mouvement et vérifier l'échec, etc. peut être ajouté ici
            placePiece(*piece, toX, toY);
            placePiece(nullptr, fromX, fromY);  // Retirer la pièce de l'ancienne position
            return true;
        }
        return false;
    }
};

#endif // BOARD_HPP
