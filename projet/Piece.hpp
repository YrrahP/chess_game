#ifndef PIECE_HPP
#define PIECE_HPP

#include <utility> // Pour std::pair

using namespace std;

enum class Color {
    White,
    Black
};


class Piece {
    public:
        Piece(Color color, int x, int y) : color(color), position(x, y) {};
        virtual ~Piece() = default;
        virtual bool isValidMove(int xNew, int yNew) const = 0;
        pair<int, int> getPosition() const { return position; }
        Color getColor() const { return color; }
        void setPosition(int x, int y) { position = { x, y }; }

    protected:
        Color color;
        pair<int, int> position;


};



#endif // PIECE_HPP