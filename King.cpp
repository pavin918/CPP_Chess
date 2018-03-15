#include "King.hpp"

//Create an unordered set that holds all possible moves in terms of (row, col) pairs
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> King::valid_moves(std::string game_board[][8])
{
	std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_spots;

	for(int r = -1; r < 2; r++)
	{
		int new_row = this->position.first + r;
		for(int c = -1; c < 2; c++)
		{
			if(r == 0 && c == 0)
			{
				continue;
			}
			int new_col = this->position.second + c;
			if(new_row > -1 && new_row < 8 && new_col > -1 && new_col < 8 && game_board[new_row][new_col][0] != this->str_id[0])
			{
				valid_spots.emplace(std::pair<int, int>(new_row, new_col));
			}
		}
	}
	return valid_spots;
}