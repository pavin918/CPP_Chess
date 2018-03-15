#include "ChessUI.hpp"

void ChessUI::init_game()
{
	/*std::string p1_input("");
	int p1_color;

	//Ensure the player enters 1 or 2
	while(p1_input < "1" || p1_input > "2")
	{
		std::cout << "Player 1, would you like to be (1)White or (2)Black? (White goes first)" << std::endl;
		std::cout << "Press '1' for White. Press '2' for Black" << std::endl;
		std::getline(std::cin, p1_input);
	}
	std::stringstream(p1_input) >> p1_color;
	return p1_color;*/
	std::cout << "Player 1 is white. Player 2 is black. Player 1 goes first. Ready? FIGHT!" << std::endl;
}

//Print out each piece on the board
void ChessUI::display_board(const std::string game_board[][8])
{
	std::cout << " 1 " << ", 2 " << ", 3 " << ", 4 " << ", 5 " << ", 6 " << ", 7 " << ", 8 " << std::endl;
	for(int row = 0; row < 8; row++)
	{
		for(int col = 0; col < 7; col++)
		{
			std::cout << game_board[row][col] << ",";
		}
		std::cout << game_board[row][7] << " " << (row + 1) << std::endl;
	}
}

std::pair<int, int> ChessUI::query_row_col(const std::string& row_query, const std::string& col_query)
{
	std::string target_row("0");
	std::string target_col("0");
	int t_row = 0;
	int t_col = 0;

	while(t_row < 1 || t_row > 8)
	{
		std::cout << row_query << std::endl;
		std::getline(std::cin, target_row);
		std::stringstream(target_row) >> t_row;
	}
	
	while(t_col < 1 || t_col > 8)
	{
		std::cout << col_query << std::endl;
		std::getline(std::cin, target_col);
		std::stringstream(target_col) >> t_col;
	}

	/*std::cout << "Player " << turn + 1 << ", which one of your pieces would you like to move? (Type the piece, as displayed on the board, you want to move)" << std::endl;
	std::getline(std::cin, target_piece);*/
	
	return std::pair<int, int>(t_row - 1, t_col - 1);
}