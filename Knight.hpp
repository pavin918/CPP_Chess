#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

class Knight: public Piece
{
	public:
		Knight(int id, std::string string_id, int row, int col) : Piece(id, string_id, row, col)
		{
		}
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves(std::string game_board[][8]);
};

#endif