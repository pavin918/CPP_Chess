#include "Player.hpp"

Player::Player(int id) : id(id), check(0), piece_count_map({{'P', 8}, {'R', 2}, {'K', 2}, {'B', 2}, {'Q', 1}}), undo_stack()
{
	if(id == 1)
	{
		color = 'W';
		piece_map = {
				{"WP1", new Pawn(id, "WP1", 6, 0)}, {"WP2", new Pawn(id, "WP2", 6, 1)}, 
				{"WP3", new Pawn(id, "WP3", 6, 2)}, {"WP4", new Pawn(id, "WP4", 6, 3)}, 
				{"WP5", new Pawn(id, "WP5", 6, 4)}, {"WP6", new Pawn(id, "WP6", 6, 5)},
				{"WP7", new Pawn(id, "WP7", 6, 6)}, {"WP8", new Pawn(id, "WP8", 6, 7)},
				{"WR1", new Rook(id, "WR1", 7, 0)}, {"WK1", new Knight(id, "WK1", 7, 1)},
				{"WB1", new Bishop(id, "WB1", 7, 2)}, {"WQN", new Queen(id, "WQN", 7, 3)}, 
				{"WKG", new King(id, "WKG", 7, 4)}, {"WB2", new Bishop(id, "WB2", 7, 5)},
				{"WK2", new Knight(id, "WK2", 7, 6)}, {"WR2", new Rook(id, "WR2", 7, 7)}
				};
	}
	else if(id == -1)
	{
		color = 'B';
		piece_map = {
				{"BP1", new Pawn(id, "BP1", 1, 0)}, {"BP2", new Pawn(id, "BP2", 1, 1)},
				{"BP3", new Pawn(id, "BP3", 1, 2)}, {"BP4", new Pawn(id, "BP4", 1, 3)}, 
				{"BP5", new Pawn(id, "BP5", 1, 4)}, {"BP6", new Pawn(id, "BP6", 1, 5)},
				{"BP7", new Pawn(id, "BP7", 1, 6)}, {"BP8", new Pawn(id, "BP8", 1, 7)},
				{"BR1", new Rook(id, "BR1", 0, 0)}, {"BK1", new Knight(id, "BK1", 0, 1)},
				{"BB1", new Bishop(id, "BB1", 0, 2)}, {"BQN", new Queen(id, "BQN", 0, 3)}, 
				{"BKG", new King(id, "BKG", 0, 4)}, {"BB2", new Bishop(id, "BB2", 0, 5)},
				{"BK2", new Knight(id, "BK2", 0, 6)}, {"BR2", new Rook(id, "BR2", 0, 7)}
				};
	}
}

Player::~Player()
{
	for(std::unordered_map<std::string, Piece*>::iterator it = piece_map.begin(); it != piece_map.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
		piece_map.erase(it->first);
	}
	while(!undo_stack.empty())
	{
		delete undo_stack.front().second;
		undo_stack.front().second = nullptr;
		undo_stack.pop_front();
	}
}

Player::Player(const Player& a_player) : id(a_player.get_id()), check(a_player.get_check()), color(a_player.get_color()), piece_count_map(a_player.get_piece_count_map())
{
	std::forward_list<std::pair<std::string, Piece*>> copied_undo_stack(a_player.get_undo_stack());
	std::forward_list<std::pair<std::string, Piece*>>::iterator undo_stack_it = undo_stack.before_begin();
	for(std::forward_list<std::pair<std::string, Piece*>>::iterator it = copied_undo_stack.begin(); it != copied_undo_stack.end(); it++)
	{
		Piece* chess_piece;
		std::pair<int, int> pos(it->second->get_position());
		if(it->first[1] == 'P')
		{
			chess_piece = new Pawn(id, it->first, pos.first, pos.second);
		}
		else if(it->first[1] == 'R')
		{
			chess_piece = new Rook(id, it->first, pos.first, pos.second);
		}
		else if(it->first[1] == 'K')
		{
			if(it->first[2] == 'G')
			{
				chess_piece = new King(id, it->first, pos.first, pos.second);
			}
			else
			{
				chess_piece = new Knight(id, it->first, pos.first, pos.second);
			}
		}
		else if(it->first[1] == 'B')
		{
			chess_piece = new Bishop(id, it->first, pos.first, pos.second);
		}
		else if(it->first[1] == 'Q')
		{
			chess_piece = new Queen(id, it->first, pos.first, pos.second);
		}
		undo_stack_it = undo_stack.insert_after(undo_stack_it, std::pair<std::string, Piece*>(it->first, chess_piece));
	}
	for(std::pair<std::string, Piece*> item : a_player.get_map())
	{
		std::pair<int, int> pos(item.second->get_position());
		if(item.first[1] == 'P')
		{
			piece_map[item.first] = new Pawn(id, item.first, pos.first, pos.second);
		}
		else if(item.first[1] == 'R')
		{
			piece_map[item.first] = new Rook(id, item.first, pos.first, pos.second);
		}
		else if(item.first[1] == 'K')
		{
			if(item.first[2] == 'G')
			{
				piece_map[item.first] = new King(id, item.first, pos.first, pos.second);
			}
			else
			{
				piece_map[item.first] = new Knight(id, item.first, pos.first, pos.second);
			}
		}
		else if(item.first[1] == 'B')
		{
			piece_map[item.first] = new Bishop(id, item.first, pos.first, pos.second);
		}
		else if(item.first[1] == 'Q')
		{
			piece_map[item.first] = new Queen(id, item.first, pos.first, pos.second);
		}
	}
}

Player& Player::operator=(const Player& a_player)
{
	if(this != &a_player)
	{
		id = a_player.get_id();
		check = a_player.get_check();
		color = a_player.get_color();
		piece_count_map = a_player.get_piece_count_map();

		for(std::unordered_map<std::string, Piece*>::iterator it = piece_map.begin(); it != piece_map.end(); it++)
		{
			delete it->second;
			it->second = nullptr;
			piece_map.erase(it->first);
		}
		while(!undo_stack.empty())
		{
			delete undo_stack.front().second;
			undo_stack.front().second = nullptr;
			undo_stack.pop_front();
		}

		std::forward_list<std::pair<std::string, Piece*>> copied_undo_stack(a_player.get_undo_stack());
		std::forward_list<std::pair<std::string, Piece*>>::iterator undo_stack_it = undo_stack.before_begin();
		for(std::forward_list<std::pair<std::string, Piece*>>::iterator it = copied_undo_stack.begin(); it != copied_undo_stack.end(); it++)
		{
			Piece* chess_piece;
			std::pair<int, int> pos(it->second->get_position());
			if(it->first[1] == 'P')
			{
				chess_piece = new Pawn(id, it->first, pos.first, pos.second);
			}
			else if(it->first[1] == 'R')
			{
				chess_piece = new Rook(id, it->first, pos.first, pos.second);
			}
			else if(it->first[1] == 'K')
			{
				if(it->first[2] == 'G')
				{
					chess_piece = new King(id, it->first, pos.first, pos.second);
				}
				else
				{
					chess_piece = new Knight(id, it->first, pos.first, pos.second);
				}
			}
			else if(it->first[1] == 'B')
			{
				chess_piece = new Bishop(id, it->first, pos.first, pos.second);
			}
			else if(it->first[1] == 'Q')
			{
				chess_piece = new Queen(id, it->first, pos.first, pos.second);
			}
			undo_stack_it = undo_stack.insert_after(undo_stack_it, std::pair<std::string, Piece*>(it->first, chess_piece));
		}
		for(std::pair<std::string, Piece*> item : a_player.get_map())
		{
			std::pair<int, int> pos(item.second->get_position());
			if(item.first[1] == 'P')
			{
				piece_map[item.first] = new Pawn(id, item.first, pos.first, pos.second);
			}
			else if(item.first[1] == 'R')
			{
				piece_map[item.first] = new Rook(id, item.first, pos.first, pos.second);
			}
			else if(item.first[1] == 'K')
			{
				if(item.first[2] == 'G')
				{
					piece_map[item.first] = new King(id, item.first, pos.first, pos.second);
				}
				else
				{
					piece_map[item.first] = new Knight(id, item.first, pos.first, pos.second);
				}
			}
			else if(item.first[1] == 'B')
			{
				piece_map[item.first] = new Bishop(id, item.first, pos.first, pos.second);
			}
			else if(item.first[1] == 'Q')
			{
				piece_map[item.first] = new Queen(id, item.first, pos.first, pos.second);
			}
		}
	}
	return *this;
}

//Return an unordered set of positions that the opponent's piece and this player's piece shares
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Player::intersect(const std::unordered_set<std::pair<int, int>, pair_hash<int, int>>& opp_moves, std::unordered_set<std::pair<int, int>, pair_hash<int, int>> my_moves)
{
	for(auto it = my_moves.begin(); it != my_moves.end();)
	{
		if(!opp_moves.count(*it))
		{
			it = my_moves.erase(it);
		}
		else
		{
			it++;
		}
	}
	return my_moves;
}

//Returns a map of pieces that can move and where they can move
std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> Player::valid_pieces_and_moves(int check, const std::string& recently_moved_piece, Player& opponent, std::string game_board[][8])
{
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> piece_moves_map;
	std::string my_king(1, this->color);
	my_king += "KG"; 
	//std::cout << "at start of valid pieces and moves" << std::endl;//debug
	for(const std::pair<std::string, Piece*>& piece : piece_map)
	{
		//Get the valid moves of the given piece
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> moves(check_piece_moves(piece.first, game_board));

		//Check if "en passant" is possible
		//If the opponent just moved a pawn two spaces forward, check if the current piece in this iteration is a pawn
		//that is adjacent to the opponent's recently moved pawn
		//If those conditions are met, add the space "behind" the opponent's pawn to this player's pawn's set of valid moves
		if(recently_moved_piece[1] == 'P' && opponent.get_pawn_two_spaces(recently_moved_piece) && piece.first[1] == 'P')
		{
			//std::cout << "in available moves map and checking for en passant, recently moved is " << recently_moved_piece << std::endl;
			std::pair<int, int> adj_pos(opponent.get_piece_position(recently_moved_piece));
			std::pair<int, int> my_pawn_pos(piece.second->get_position());
			if(my_pawn_pos.first == adj_pos.first && (my_pawn_pos.second == adj_pos.second - 1 || my_pawn_pos.second == adj_pos.second + 1))
			{
				moves.insert(std::pair<int, int>(my_pawn_pos.first - this->id, adj_pos.second));
			}
		}

		//If the piece in the current iteration can't move anywhere, move to the next piece
		if(moves.empty())
		{
			//std::cout << "moves empty" << std::endl;//debug
			continue;
		}

		//Avoid infinite recursion when doing 1-turn lookahead
		//1-turn lookahead checks...
		//if a piece is currently blocking an opponent's move that would take out a king,
		//and if a piece could block an opponent's piece that is causing check
		//If recently_moved_piece is not "   ", it is implied that this function has been called again to do 1-turn
		//lookahead, and the perspective has been switched to the opponent.
		//Switching to the opponent's perspective is important because if this player moves a piece, it is 
		//possible that this player is either now or still in check. Whether this player makes a move that puts
		//the opponent in check is of no importance to this player
		if(recently_moved_piece != "   ")//check || piece.first == my_king)
		{
			//std::cout << "not doing forward checking" << piece.first << std::endl;//debug
			//std::unordered_set<std::pair<int, int>, pair_hash<int, int>> opp_and_moves(opponent.check_piece_moves(recently_moved_piece, game_board));
			
			//Except for this player's king, 
			//Get positions where the current piece's valid moves and the valid moves of the opponent's piece that put this player in check intersect
			//to try to block the opponent's piece and get out of check
			if(check && piece.first != my_king)//else
			{
				//std::unordered_set<std::pair<int, int>, pair_hash<int, int>> opp_and_moves(opponent.check_piece_moves(recently_moved_piece, game_board));
				std::unordered_set<std::pair<int, int>, pair_hash<int, int>> opp_and_moves(opponent.check_piece_moves(recently_moved_piece, game_board));
				opp_and_moves.insert(opponent.get_piece_position(recently_moved_piece));
				moves = intersect(opp_and_moves, moves);
			}

			for(std::unordered_set<std::pair<int, int>, pair_hash<int, int>>::iterator move_it = moves.begin(); move_it != moves.end();)
			{
				//Will be raised in the upcoming for loop if the pair "move_it" points to has been erased
				unsigned int erased_flag = 0;

				//Store the current piece's position to later undo the move that will get this player out of check
				//Check if, after hypothetically applying this move, the king is still in check b/c it is possible
				//for the piece in this iteration to currently block another piece that could cause a check.
				//In addition, it is possible for this player's king to eat the piece that caused the check,
				//but eating that piece may still place the king in check

				std::pair<int, int> curr_pos(piece.second->get_position());
				std::pair<int, int> new_pos(move_it->first, move_it->second);
				std::string replaced_piece(game_board[move_it->first][move_it->second]);
				
				piece.second->set_position(move_it->second, move_it->first);
				game_board[curr_pos.first][curr_pos.second] = "   ";
				game_board[move_it->first][move_it->second] = piece.first;
				std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> all_opp_moves(opponent.valid_pieces_and_moves(0, "   ", *this, game_board));
				all_opp_moves.erase(replaced_piece);

				for(const std::pair<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>>& opp_piece_moves : all_opp_moves)
				{
					if(opp_piece_moves.second.count(piece_map[my_king]->get_position()))
					{
						//remove move from moves
						move_it = moves.erase(move_it);
						erased_flag = 1;
						break;
					}
				}
				game_board[new_pos.first][new_pos.second] = replaced_piece;
				piece.second->set_position(curr_pos.second, curr_pos.first);
				game_board[curr_pos.first][curr_pos.second] = piece.first;
				if(!erased_flag)
				{
					move_it++;
				}
			}
		}
		//Check if, after doing forward checking, unordered set "moves" is empty or not.
		//If not, add the piece-moves pair to the map that will be returned 
		if(!moves.empty())
		{
			piece_moves_map[piece.first] = moves;
		}
	}
	if(recently_moved_piece != "   " && !check)
	{
		//Check if castling is a valid option
		//Castling can only happen if...
		//1)the king and a rook have not moved
		//2)there are no pieces in between the king and rook
		//3)the player is not in check
		//4)moving the king to any of the spaces between its starting and end position before and after castling will not cause a check

		King* king_piece = dynamic_cast<King*>(piece_map[my_king]);
		if(!king_piece->has_moved()) 
		{
			std::string my_rook1(1, this->color);
			my_rook1 += "R1";
			Rook* rook_piece1 = dynamic_cast<Rook*>(piece_map[my_rook1]);

			std::string my_rook2(1, this->color);
			my_rook2 += "R2";
			Rook* rook_piece2 = dynamic_cast<Rook*>(piece_map[my_rook2]);

			std::pair<int, int> king_pos = piece_map[my_king]->get_position();
			int king_row = king_pos.first;
			int kingside_col = king_pos.second;
			if(!rook_piece1->has_moved()) 
			{
				int castling_invalid = 0;
				
				std::pair<int, int> queenside_spot1(king_row, 2);
				std::pair<int, int> queenside_spot2(king_row, 3);
				if(game_board[king_row][1] == "   " && game_board[king_row][2] == "   " && game_board[king_row][3] == "   ")
				{
					std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> opp_piece_moves_map(opponent.valid_pieces_and_moves(0, "   ", *this, game_board));
					for(const std::pair<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>>& opp_piece_moves : opp_piece_moves_map)
					{
						if(opp_piece_moves.second.count(queenside_spot1) || opp_piece_moves.second.count(queenside_spot2))
						{
							castling_invalid = 1;
							break;
						}
					}
				}
				else
				{
					castling_invalid = 1;
				}
				if(!castling_invalid)
				{
					piece_moves_map[my_king].insert(queenside_spot1);
					piece_moves_map[my_rook1].insert(queenside_spot2);
				}
			}
			if(!rook_piece2->has_moved())
			{
				int castling_invalid = 0;
				
				std::pair<int, int> kingside_spot1(king_row, 6);
				std::pair<int, int> kingside_spot2(king_row, 5);
				if(game_board[king_row][5] == "   " && game_board[king_row][6] == "   ")
				{
					std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> opp_piece_moves_map(opponent.valid_pieces_and_moves(0, "   ", *this, game_board));
					for(const std::pair<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>>& opp_piece_moves : opp_piece_moves_map)
					{
						if(opp_piece_moves.second.count(kingside_spot1) || opp_piece_moves.second.count(kingside_spot2))
						{
							castling_invalid = 1;
							break;
						}
					}
				}
				else
				{
					castling_invalid = 1;
				}
				if(!castling_invalid)
				{
					piece_moves_map[my_king].insert(kingside_spot1);
					piece_moves_map[my_rook1].insert(kingside_spot2);
				}
			}
		}
	}
	return piece_moves_map;
}

//Method called only for Kings, Rooks, and Pawns because they have special moves that depend on if they've moved or not
int Player::get_moved(const std::string& piece)
{
	if(piece[1] == 'P')
	{
		dynamic_cast<Pawn*>(piece_map[piece])->has_moved();
	}
	else if(piece[1] == 'K' && piece[2] == 'G')
	{
		dynamic_cast<King*>(piece_map[piece])->has_moved();
	}
	else if(piece[1] == 'R')
	{
		dynamic_cast<Rook*>(piece_map[piece])->has_moved();
	}
	return 0;
}

//Method called only for Kings, Rooks, and Pawns because they have special moves that depend on if they've moved or not
void Player::raise_moved(const std::string& piece)
{
	if(piece[1] == 'P')
	{
		dynamic_cast<Pawn*>(piece_map[piece])->raise_moved();
	}
	else if(piece[1] == 'K' && piece[2] == 'G')
	{
		dynamic_cast<King*>(piece_map[piece])->raise_moved();
	}
	else if(piece[1] == 'R')
	{
		dynamic_cast<Rook*>(piece_map[piece])->raise_moved();
	}
}

//Takes out the piece from the map when it is eaten by an enemy piece
void Player::eaten(const std::string& piece)
{
	delete piece_map[piece];
	piece_map[piece] = nullptr;
	piece_map.erase(piece);
	--piece_count_map[piece[1]];
}

//If the piece passed to this function is a Pawn, promote it
std::string Player::promotion(const std::string& piece, const int& new_row, const int& new_col, const std::string& new_piece)
{
	if(piece[1] == 'P')
	{
		if((new_row == 0 && piece[0] == 'W') || (new_row == 7 && piece[0] == 'B'))
		{
			std::string new_piece_rank(1, piece[0]);
			std::stringstream ss;
			new_piece_rank += new_piece;
			ss << ++piece_count_map[new_piece[0]];
			new_piece_rank += ss.str();

			switch(new_piece[0])
			{
				case 'Q':
					piece_map[new_piece_rank] = new Queen(id, new_piece_rank, new_row, new_col);
					break;
				case 'R':
					piece_map[new_piece_rank] = new Rook(id, new_piece_rank, new_row, new_col);
					break;
				case 'K':
					piece_map[new_piece_rank] = new Knight(id, new_piece_rank, new_row, new_col);
					break;
				case 'B':
					piece_map[new_piece_rank] = new Bishop(id, new_piece_rank, new_row, new_col);
					break;
			}
			eaten(piece);
			return new_piece_rank;
		}
	}
	return piece;
}

//Put the piece that was deleted onto the undo_stack and delete it from the piece_map, but do not delete
//the Piece on the heap. Create a 2nd pointer to that Piece that will be stored in the undo_stack, and delete
//the pointer in the piece_map
void Player::push_and_delete(const std::string piece)
{
	if(piece_map.count(piece))
	{
		undo_stack.push_front(std::pair<std::string, Piece*>(piece, piece_map[piece]));
		piece_map.erase(piece);
	}
}

//Undo the death of the most recently killed piece this player owns
void Player::restore_piece()
{
	if(!undo_stack.empty())
	{
		std::pair<std::string, Piece*> piece = undo_stack.front();
		piece_map[piece.first] = piece.second;
		undo_stack.pop_front();
	}
}

/*
void Player::forward_check_1turn(const std::pair<std::string, Piece*>& piece, Player& opponent, std::unordered_set<int, pair_hash<int, int>>& moves, std::string game_board[][8])
{
	for(std::pair<int, int>::iterator move_it = moves.begin(); move_it != moves.end();)
	{
		//Will be raised in the upcoming for loop if the pair "move_it" points to has been erased
		unsigned int erased_flag = 0;

		//Store the current piece's position to later undo the move that will get this player out of check
		//Check if, after hypothetically applying this move, the king is still in check b/c it is possible
		//for the piece in this iteration to currently block another piece that could cause a check
		//Store the string that is in the position this piece will move to

		std::pair<int, int> curr_pos(piece.second->get_position());
		std::pair<int, int> new_pos(move_it->first, move_it->second);
		std::string replaced_piece(game_board[move_it->first][move_it->second]);
			
		piece.second->set_position(move_it->second, move_it->first);
		game_board[curr_pos.first][curr_pos.second] = "   ";
		game_board[move_it->first][move_it->second] = piece.first;
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> all_opp_moves(opponent.valid_pieces_and_moves(0, piece.first, *this, game_board));
		all_opp_moves.erase(replaced_piece);
		
		for(const std::pair<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>>& opp_piece_moves : all_opp_moves)
		{
			if(opp_piece_moves.second.count(piece_map[my_king]->get_position()))
			{
				//remove move from moves
				move_it = moves.erase(move_it);
				erased_flag = 1;
				break;
			}
		}
		
		//Restore the string that was replaced by this piece's string
		//And restore the this piece's string to its original position before the forward check
		game_board[new_pos.first][new_pos.second] = replaced_piece;
		piece.second->set_position(curr_pos.second, curr_pos.first);
		game_board[curr_pos.first][curr_pos.second] = piece.first;
		if(erased_flag)
		{
			move_it++;
		}
	}
}

*/
//Return the unordered set of valid moves for the selected piece
std::unordered_set<std::pair<int, int>, pair_hash<int, int>> Player::check_piece_moves(const std::string& str_piece, std::string game_board[][8])
{
	//Call and return the selected piece's valid moves
	return this->piece_map[str_piece]->valid_moves(game_board);
}

//Set the position of the selected piece
/*void Player::move_piece(const std::string& str_piece, const int& col, const int& row)
{
	this->piece_map[str_piece]->set_position(col, row);
}*/