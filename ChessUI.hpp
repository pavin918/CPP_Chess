#ifndef CHESSUI_HPP
#define CHESSUI_HPP

#include <iostream>
#include <sstream>

//#include "Piece.hpp"
//#include "Player.hpp"

class ChessUI
{
	public:
		//Instantiate the chess board
		//ChessUI();

		//Ask Player 1 what color he/she wants to be to start the game
		//Just tell whch player is white, which is black, and who goes first
		void init_game();
		
		//Print out the board of strings
		void display_board(const std::string game_board[][8]);

		//Ask the player(Player 1 or Player 2) what row and column of the piece he or she wants to move,
		//OR ask where would the player like to move the piece and return the row, col as a pair
		//row_query is a message that asks for the row and col_query is a message that asks for the column.
		//These messages are printed on the screen for the player.
		std::pair<int, int> query_row_col(const std::string& row_query, const std::string& col_query);
};

#endif