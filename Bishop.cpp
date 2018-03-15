#include "Bishop.hpp"

//Create an unordered set that holds all possible moves in terms of (row, col) pairs
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Bishop::valid_moves(std::string game_board[][8])
{
	//Start from the Bishop's position, and traverse the game board diagonally (NW, NE, SW, SE) to find all valid moves

	std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_spots;
	
	for(int i = -1; i < 2; i += 2)
	{
		for(int j = -1; j < 2; j += 2)
		{
			int new_row = this->position.first + i;
			int new_col = this->position.second + j;
			while(new_col > -1 && new_col < 8 && new_row > -1 && new_row < 8 && game_board[new_row][new_col][0] != this->str_id[0])
			{
				valid_spots.emplace(std::pair<int, int>(new_row, new_col));
				if(game_board[new_row][new_col][0] != ' ')
				{
					break;
				}
				new_col += j;
				new_row += i;
			}
		}
	}
	return valid_spots;		
}