#include "Rook.hpp"

//Create an unordered set that holds all possible moves in terms of (row, col) pairs
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Rook::valid_moves(std::string game_board[][8])
{
	//Start from the Rook's position, and traverse the game board in the 4 cardinal directions (N, S, W, E) to find all valid moves

	std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_spots;

	for(int i = -1; i < 2; i += 2)
	{
		//Find all possible vertical moves
		int new_row = this->position.first + i;
		while(new_row > -1 && new_row < 8 && game_board[new_row][this->position.second][0] != this->str_id[0])
		{
			valid_spots.emplace(std::pair<int, int>(new_row, this->position.second));
			if(game_board[new_row][this->position.second][0] != ' ')
			{
				break;
			}
			new_row += i;
		}
		
		//Find all possible horizontal moves
		int new_col = this->position.second + i;
		while(new_col > -1 && new_col < 8 && game_board[this->position.first][new_col][0] != this->str_id[0])
		{
			valid_spots.emplace(std::pair<int, int>(this->position.first, new_col));
			if(game_board[this->position.first][new_col][0] != ' ')
			{
				break;
			}
			new_col += i;
		}
	}
	return valid_spots;
}