#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp" 


using namespace std;

class King : public Piece {
	public:
		King(Color color, int x, int y) : Piece(color, x, y) {}

		bool isValidMove(int xNew, int yNew) const override {
			int dx = abs(xNew - position.first);
			int dy = abs(yNew - position.second);
			return (dx <= 1 && dy <= 1) && (dx != 0 || dy != 0);
		}

};

#endif //KING_HPP
