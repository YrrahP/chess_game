//#ifndef ROOK_HPP
//#define ROOK_HPP
//
//#include "Piece.hpp"
//
//using namespace std;
//
//class Rook : public Piece {
//public:
//    Rook(Color color, int x, int y) : Piece(color, x, y) {}
//
//    bool isValidMove(int xNew, int yNew) const override {
//        int dx = abs(xNew - position.first);
//        int dy = abs(yNew - position.second);
//        return (dx == 0 && dy != 0) || (dx != 0 && dy == 0);
//    }
//};
//
//#endif // ROOK_HPP
