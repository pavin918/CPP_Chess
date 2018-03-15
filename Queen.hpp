#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Bishop.hpp"
#include "Rook.hpp"

class Queen: public Bishop, public Rook
{
	public:
		Queen(int id, std::string string_id, int row, int col) : Piece(id, string_id, row, col), Bishop(id, string_id, row, col), Rook(id, string_id, row, col)
		{
		}

		//Create an unordered set that holds all possible moves in terms of (row, col) pairs
		//Queen's valid moves are a fusion of Bishop's and Rook's
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves(std::string game_board[][8])
		{
			std::unordered_set<std::pair<int, int>, pair_hash<int, int>> my_moves(Bishop::valid_moves(game_board));
			std::unordered_set<std::pair<int, int>, pair_hash<int, int>> rook_moves(Rook::valid_moves(game_board));
			my_moves.insert(rook_moves.begin(), rook_moves.end());
			return my_moves;
		}
};

#endif