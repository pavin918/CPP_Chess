#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

class King: public Piece
{
	public:
		King(int id, std::string string_id, int row, int col) : Piece(id, string_id, row, col), moved(0)
		{
		}

		int has_moved()
		{
			return moved;
		}

		void raise_moved()
		{
			moved = 1;
		}

		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves(std::string game_board[][8]);
	private:
		int moved;
};

#endif