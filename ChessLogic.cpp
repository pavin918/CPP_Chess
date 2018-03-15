#include "ChessLogic.hpp"

ChessLogic::ChessLogic() : current_status(normal), ui(), turn(0), players{Player(1), Player(-1)},
		     game_board{{"BR1", "BK1", "BB1", "BQN", "BKG", "BB2", "BK2", "BR2"},
				{"BP1", "BP2", "BP3", "BP4", "BP5", "BP6", "BP7", "BP8"},
				{"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
				{"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
				{"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
				{"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
				{"WP1", "WP2", "WP3", "WP4", "WP5", "WP6", "WP7", "WP8"},
				{"WR1", "WK1", "WB1", "WQN", "WKG", "WB2", "WK2", "WR2"}}
{
}

void ChessLogic::run_game()
{
	//"recently_moved" is the string representing a piece that was just moved.
	std::string recently_moved = "   ";

	while(1)
	{
		ui.display_board(game_board);
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> available_moves_map(players[turn].valid_pieces_and_moves(current_status, recently_moved, players[(turn + 1) % 2], game_board));
		//std::cout << "got available maps" << std::endl;//debug
		if(available_moves_map.empty())
		{
			if(current_status == check)
			{
				std::cout << "Checkmate! Player " << alternate_turn() << ", you win!" << std::endl;
				return;
			}
			else
			{
				std::cout << "Player " << (turn + 1) << ", is not in check and cannot move. It's a draw" << std::endl;
				return;
			}
		}

		if(current_status == check)
		{
			std::cout << "Player " << turn + 1 << ", you are in check." << std::endl;
		}
		std::cout << "Player " << turn + 1 << ", it is your turn." << std::endl;

		//Print out the pieces the "current" player can move and their positions so the player can
		//choose the position of the piece he/she wants to move

		std::cout << "The pieces you can move and their positions are..." << std::endl;
		for(const std::pair<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>>& piece_moves : available_moves_map)
		{
			std::cout << piece_moves.first << " : ";
			const std::pair<int, int>& piece_pos = players[turn].get_piece_position(piece_moves.first);
			std::cout << "(" << piece_pos.first + 1 << ", " << piece_pos.second + 1 << ")" << std::endl;
		}
		std::cout << std::endl;

		//Asks the position of the piece the player wants to move
		//Asks for the row first; valid answer is 1-8 with no white space
		//Then asks for the column; valid answer is 1-8 with no white space
		//Valid answer is one of the strings that represents one of the player's pieces on the board

		std::pair<int, int> selected_piece_pos = which_piece();
		std::string selected_piece = game_board[selected_piece_pos.first][selected_piece_pos.second];
		while(!available_moves_map.count(selected_piece))
		{
			std::cout << "The row and column you entered does not correspond to a piece you can move. Try again." << std::endl;
			selected_piece_pos = which_piece();
			selected_piece = game_board[selected_piece_pos.first][selected_piece_pos.second];
		}

		//Print out where the player can move the selected piece
		//Then, ask which row the player wants to move their piece
		//Valid answer is 1-8 with no white space
		//Then, ask which column the player wants to move their piece
		//Valid answer is 1-8 with no white space

		std::cout << "The positions you can move " << selected_piece << "[in (row, col) form] are..." << std::endl;
		const std::unordered_set<std::pair<int, int>, pair_hash<int, int>>& piece_valid_moves = available_moves_map[selected_piece];
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>>::const_iterator pvm_it = piece_valid_moves.begin();
		std::cout << "(" << pvm_it->first + 1 << ", " << pvm_it->second + 1 << ")";
		while(++pvm_it != piece_valid_moves.end())
		{
			std::cout << ", (" << pvm_it->first + 1 << ", " << pvm_it->second + 1 << ")";
		}
		std::cout << std::endl;

		std::pair<int, int> move_here = move_where();

		//If the row, col pair does not match a valid spot the piece can move to,
		//ask the player again which row and column the player wants to move their piece.
		//Note: the player cannot undo which piece they chose to move		
		while(!available_moves_map[selected_piece].count(move_here))
		{
			std::cout << "You cannot move " << selected_piece << " to (" << move_here.first + 1 << ", " << move_here.second + 1 << "). Please try again." << std::endl;
			move_here = move_where();
		}
			
		//Change the state of the board, the state of the player(s)' piece maps, 
		//and the state of the pieces affected by the "current" player's actions

		std::string target_piece = game_board[move_here.first][move_here.second];
		players[turn].move_piece(selected_piece, move_here.second, move_here.first);
		game_board[selected_piece_pos.first][selected_piece_pos.second] = "   ";
		game_board[move_here.first][move_here.second] = selected_piece;

		//std::cout << "Board and player's piece info updated" << std::endl;
		
		//std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves = players[turn].check_piece_moves(selected_piece, game_board);
		int current_turn = turn;
		alternate_turn();
		if(target_piece != "   ")
		{
			//Delete the piece that was just eaten from the "current" player's map
			players[turn].eaten(target_piece);
		}
		else
		{
			//This checks for "en passant." If Player A moved a pawn two spaces forward, Player B
			//had a pawn to the immediate left or immediate right of the pawn Player A just moved,
			//and Player B moved his/her pawn diagonally, positioning his/her pawn "behind" Player A's
			//pawn, delete Player A's pawn from his/her piece map and replace the string representing
			//that pawn on the board with "   ".
			//The "get_pawn_two_spaces" function checks if the string passed to it is a pawn or not, and
			//if it is, returns whether it has just moved two spaces or not
			if(players[turn].get_pawn_two_spaces(recently_moved))
			{
				std::pair<int, int> passed_pawn_pos = players[turn].get_piece_position(recently_moved);
				//behind_pawn is the spot just "behind" the current player's pawn after he/she moved it two spaces "forward" in his/her last turn
				int behind_pawn = passed_pawn_pos.first + players[turn].get_id();
				if(move_here.first == behind_pawn && move_here.second == passed_pawn_pos.second)
				{
					players[turn].eaten(recently_moved);
					game_board[passed_pawn_pos.first][passed_pawn_pos.second] = "   ";
				}
			}
			//This statement block checks if castling was done.
			//Note: Castling is considered a move the king can make. Therefore, to castle, the player
			//must choose to move the king, not the rook.
			else if(!players[current_turn].get_moved(selected_piece))
			{
				std::cout << "checking for castling" << std::endl;
				int castle_row = ((selected_piece[0] == 'W' ? 7 : 0));
				if(selected_piece[1] == 'K' && selected_piece[2] == 'G')
				{
					std::string rook(1, selected_piece[0]);
					rook += "R";
					if(move_here.second == 6)
					{
						rook += "2";
						if(!players[current_turn].get_moved(rook))
						{
							players[current_turn].move_piece(rook, 5, castle_row);
							players[current_turn].raise_moved(rook);
							game_board[castle_row][5] = rook;
							game_board[castle_row][7] = "   ";
						}
					}
					else if(move_here.second == 2)
					{
						rook += "1";
						if(!players[current_turn].get_moved(rook))
						{
							players[current_turn].move_piece(rook, 3, castle_row);
							players[current_turn].raise_moved(rook);
							game_board[castle_row][3] = rook;
							game_board[castle_row][0] = "   ";
						}
					}
				}
			}
		}

		//std::cout << "Check for en passant and castling complete" << std::endl;
		if(selected_piece[1] == 'P' && ((selected_piece[0] == 'W' && move_here.first == 0) || (selected_piece[0] == 'B' && move_here.first == 7)))
		{
			std::cout << "Congratulations! Your pawn has made it alive in enemy territory! You can now promote your pawn!" << std::endl;
			std::string promotion_options("What would you like to promote your pawn to?\nPress 'Q' for Queen.\nPress 'R' for Rook.\nPress 'B' for Bishop.\nPress 'K' for Knight.");
			std::string promotion_choice = "";
			while(promotion_choice != "Q" && promotion_choice != "R" && promotion_choice != "B" && promotion_choice != "K")
			{
				std::cout << promotion_options << std::endl;
				getline(std::cin, promotion_choice);
			}	
			selected_piece = players[current_turn].promotion(selected_piece, move_here.first, move_here.second, promotion_choice);
			game_board[move_here.first][move_here.second] = selected_piece;
		}

		//If the piece this player moved in his/her last turn is a Pawn, lower the flag that indicates
		//this player moved his/her pawn two spaces "forward"
		players[turn].lower_two_spaces(recently_moved);

		//Only the piece that was just moved can cause the current player to be in check
		//Check if the current player is now in check
		std::string king;
		std::stringstream ss;
		ss << players[turn].get_color() << "KG";
		ss >> king;
		const std::unordered_set<std::pair<int, int>, pair_hash<int, int>>& selected_piece_moves = players[current_turn].check_piece_moves(selected_piece, game_board);
		current_status = (selected_piece_moves.count(players[turn].get_piece_position(king)) ? check : normal);

		//Raise two_spaces for Pawns if they were moved this turn or moved last turn, respectively
		int other_player = (turn + 1) % 2;
		if(turn == 0)
		{				
			if(selected_piece[1] == 'P' && !players[current_turn].get_moved(selected_piece) && move_here.first == 3)
			{
				players[current_turn].raise_two_spaces(selected_piece);
			}	
		}
		else
		{
			if(selected_piece[1] == 'P' && !players[current_turn].get_moved(selected_piece) && move_here.first == 4)
			{
				players[current_turn].raise_two_spaces(selected_piece);
			}
		}

		//Raise recently_moved for Rooks, Kings, and Pawns if they were moved this turn
		//or moved last turn, respectively
		players[current_turn].raise_moved(selected_piece);
			
		recently_moved = selected_piece;
	}
}

int ChessLogic::alternate_turn()
{
	if(this->turn == 1)
	{
		this->turn = 0;
	}
	else
	{
		this->turn = 1;
	}
	return this->turn;
}

std::pair<int, int> ChessLogic::which_piece()
{
	//In this function, query_row_col() will query the user to enter the row and column of the piece they want to move.
	//User will enter numbers, and function query_row_col will decrement those numbers for 0-based indexing, 
	//And return a pair of ints, representing row and col

	return ui.query_row_col("What is the row of the piece you would like to move?", "What is the column of the piece you would like to move?");
}

std::pair<int, int> ChessLogic::move_where()
{
	//In this function, query_row_col() will query the user to enter the row and column they want to move their piece 
	//User will enter numbers, and function query_row_col will decrement those numbers for 0-based indexing,
	//And return a pair of ints, representing row and col

	return ui.query_row_col("To what row would you like to move the selected piece?", "To what column would you like to move the selected piece?");
}