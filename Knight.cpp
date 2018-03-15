#include "Knight.hpp"

//Create an unordered set that holds all possible moves in terms of (row, col) pairs
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Knight::valid_moves(std::string game_board[][8])
{
	std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_spots;
	
	int adj[4] = {-2, -1, 1, 2};
	for(int index = 0; index < 4; index++)
	{
		int new_row = this->position.first + adj[index];
		for(int h_sign = -1; h_sign < 2; h_sign += 2)
		{
			int horizontal_movement = ((adj[index] << 1) % 3) * h_sign;
			int new_col = this->position.second + horizontal_movement;
			if(new_col > -1 && new_col < 8 && new_row > -1 && new_row < 8 && game_board[new_row][new_col][0] != this->str_id[0])
			{
				valid_spots.emplace(std::pair<int, int>(new_row, new_col));
			}
		}
	}
	return valid_spots;
}