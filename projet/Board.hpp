#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Rook.hpp"
#include <memory>
#include <vector>
#include <optional>

using namespace std;

class Board {
private:
    vector<vector<shared_ptr<Piece>>> grid;
    static const int size = 8;

public:
    Board() {
        grid.resize(size, vector<shared_ptr<Piece>>(size, nullptr));
    }

    void placePiece(const shared_ptr<Piece>& piece, int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size) {
            grid[x][y] = piece;
        }
    }

    optional<shared_ptr<Piece>> getPiece(int x, int y) const {
        if (x >= 0 && x < size && y >= 0 && y < size) {
            return grid[x][y];
        }
        return nullopt;
    }

        bool movePiece(int fromX, int fromY, int toX, int toY) {
            auto piece = getPiece(fromX, fromY);
            if (piece && *piece && (*piece)->isValidMove(toX, toY)) {
                placePiece(*piece, toX, toY);
                placePiece(nullptr, fromX, fromY);
                return true;
            }
            return false;
        }
};

#endif // BOARD_HPP
