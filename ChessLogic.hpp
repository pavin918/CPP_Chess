#ifndef CHESSLOGIC_HPP
#define CHESSLOGIC_HPP

#include "ChessUI.hpp"
#include "Player.hpp"

class ChessLogic
{
	public:
		ChessLogic();

		//Function that runs the game until a checkmate occurs
		void run_game();
		
	private:
		//Change and return member variable "turn," the index for array "players"
		//"turn" can only be 0 or 1 
		int alternate_turn();

		//Ask the player the row and column of the piece the player wants to move and return them as a pair
		//Check if the row, col pair corresponds to a piece the player owns
		//If the player does not own the piece, query the player again
		//If the player does own the piece, return the string representing the piece
		std::pair<int, int> which_piece();

		//Ask the player the row and column the player wants to move the piece to and return them as a pair
		std::pair<int, int> move_where();

		enum game_status{normal, check, draw, checkmate};

		game_status current_status;
		ChessUI ui;
		/*int check;		
		int checkmate;
		int draw;*/
		int turn;
		Player players[2];
		std::string game_board[8][8];
};

#endif