#ifndef PAWN_HPP
#define PAWN_HPP

#include "Piece.hpp"

class Pawn: public Piece
{
	public:
		Pawn(int id, std::string string_id, int row, int col) : Piece(id, string_id, row, col), moved(0), two_spaces(0)
		{
		}
		
		int has_moved() const
		{
			return moved;
		}

		int has_moved_two_spaces() const
		{
			return two_spaces;
		}

		void raise_moved()
		{
			moved = 1;
		}

		void raise_two_spaces()
		{
			two_spaces = 1;
		}

		void lower_two_spaces()
		{
			two_spaces = 0;
		}
		
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves(std::string game_board[][8]);
	private:
		int moved;
		int two_spaces;
};

#endif