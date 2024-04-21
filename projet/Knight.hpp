//#ifndef KNIGHT_HPP
//#define KNIGHT_HPP
//
//#include "Piece.hpp"
//
//using namespace std;
//
//class Knight : public Piece {
//	public:
//		Knight(Color color, int x, int y) : Piece(color, x, y) {}
//
//		bool isValidMove(int xNew, int yNew) const override{
//			int dx = abs(xNew - position.first);
//			int dy = abs(yNew - position.second);
//			return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
//		}
//};
//
//#endif //KNIGHT_HPP
