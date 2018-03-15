#include "Pawn.hpp"

//Create an unordered set that holds all possible moves in terms of (row, col) pairs
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Pawn::valid_moves(std::string game_board[][8])
{
	//Because black is -1 and white is 1, the way a pawn moves forward is by taking its current row, and subtracting its id
	//because white is at the "front" and black is at the "back"

	int forward_row = this->position.first - this->id;
	std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_spots;
	
	//Check if the pawn can move forward
	if(game_board[forward_row][this->position.second][0] == ' ')
	{
		valid_spots.emplace(std::pair<int, int>(forward_row, this->position.second));
		if(!moved && game_board[forward_row - this->id][this->position.second][0] == ' ')
		{
			valid_spots.emplace(std::pair<int, int>(forward_row - this->id, this->position.second));
		}
	}

	int forward_diag1(this->position.second + 1);
	int forward_diag2(this->position.second - 1);

	std::string str_piece1(game_board[forward_row][forward_diag1]);
	std::string str_piece2(game_board[forward_row][forward_diag2]);

	//If an enemy piece is in "front" of and diagonally adjacent to this pawn, 
	//add the enemy piece's position to the list of valid moves
	if(forward_diag1 < 8 && str_piece1[0] != this->str_id[0] && str_piece1[0] != ' ') //game_board[forward_row][forward_diag1].get_id() == this.get_opp_id())
	{
		valid_spots.emplace(std::pair<int, int>(forward_row, forward_diag1));
	}
	if(forward_diag2 > -1 && str_piece2[0] != this->str_id[0] && str_piece2[0] != ' ') //game_board[forward_row][forward_diag2].get_id() == this.get_opp_id())
	{
		valid_spots.emplace(std::pair<int, int>(forward_row, forward_diag2));
	}

	return valid_spots;
}