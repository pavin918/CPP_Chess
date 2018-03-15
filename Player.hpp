#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <forward_list>

#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"
#include "ChessUI.hpp"

//Idea: use an unordered map to map string id, which represents the piece on the board, with the piece itself

//Player class defines what actions the player can take, the player's characteristics, and keeps track of the player's chess pieces on the board
class Player
{
	public:
		Player(int id);

		~Player();

		Player(const Player& a_player);

		Player& operator=(const Player& a_player);

		//Returns Player's id(or color)
		int get_id() const
		{
			return id;
		}

		//Returns "check," which is 0 if player is not in check and is 1 if player is in check
		int get_check() const
		{
			return check;
		}

		char get_color() const
		{
			return color;
		}

		std::unordered_map<char, uint8_t> get_piece_count_map() const
		{
			return piece_count_map;
		}

		std::forward_list<std::pair<std::string, Piece*>> get_undo_stack() const
		{
			return undo_stack;
		}

		

		std::unordered_map<std::string, Piece*> get_map() const
		{
			return piece_map;
		}

		//Return 1 if the string is a key in the piece map; Return 0 otherwise
		int has_piece(const std::string& str_piece) const
		{
			return piece_map.count(str_piece);
		}

		//Return the position of the specified piece
		std::pair<int, int> get_piece_position(const std::string& piece)
		{
			return piece_map[piece]->get_position();
		}

		int get_pawn_two_spaces(const std::string& piece) 
		{
			return (piece[1] == 'P' ?
				dynamic_cast<Pawn*>(piece_map[piece])->has_moved_two_spaces() : 0);
		}

		//Method that raises a flag that indicates this player just moved a pawn 2 spaces
		void raise_two_spaces(const std::string& piece)
		{
			if(piece[1] == 'P' && piece_map.count(piece))
			{
				dynamic_cast<Pawn*>(piece_map[piece])->raise_two_spaces();
			}
		}

		//Method that lowers a flag that indicates this player moved a surviving pawn 2 spaces in his/her last turn
		void lower_two_spaces(const std::string& piece)
		{
			if(piece[1] == 'P' && piece_map.count(piece))
			{
				dynamic_cast<Pawn*>(piece_map[piece])->lower_two_spaces();
			}
		}

		//Set the position of the specified Piece the string is mapped to
		void move_piece(std::string str_piece, int col, int row)
		{
			piece_map[str_piece]->set_position(col, row);
		}

		//Method called only for Kings, Rooks, and Pawns because they have special moves that depend on if they've moved or not
		int get_moved(const std::string& piece);

		//Method called only for Kings, Rooks, and Pawns because they have special moves that depend on if they've moved or not
		void raise_moved(const std::string& piece);

		//Takes out the piece from the map when it is eaten by an enemy piece
		void eaten(const std::string& piece);
		
		//If the piece passed to this function is a Pawn, promote it
		std::string promotion(const std::string& piece, const int& new_row, const int& new_col, const std::string& new_piece);

		//Put the piece that was deleted onto the undo_stack and delete it from the piece_map
		void push_and_delete(const std::string piece);

		//Undo the death of the most recently killed piece this player owns
		void restore_piece();

		//Returns a map of pieces that can move and where they can move
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, pair_hash<int, int>>> valid_pieces_and_moves(int check, const std::string& recently_moved_piece, Player& opponent, std::string game_board[][8]);
		
		//Return the list of valid moves for the selected piece
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> check_piece_moves(const std::string& str_piece, std::string game_board[][8]);

		//Move the selected chess piece to the spot specified. Set the position of the selected piece
		/*void move_piece(const std::string& str_piece, const int& col, const int& row);*/

	private:
		std::unordered_set<std::pair<int, int>, pair_hash<int, int>> intersect(const std::unordered_set<std::pair<int, int>, pair_hash<int, int>>& opp_moves, std::unordered_set<std::pair<int, int>, pair_hash<int, int>> my_moves);
		//void forward_check_1turn(const std::pair<std::string, Piece*>& piece, Player& opponent, std::unordered_set<int, pair_hash<int, int>>& moves, std::string game_board[][8]);

		int id;
		int check;
		char color;
		
		std::unordered_map<char, uint8_t> piece_count_map;
		std::forward_list<std::pair<std::string, Piece*>> undo_stack;
		std::unordered_map<std::string, Piece*> piece_map;
};

#endif