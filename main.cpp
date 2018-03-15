#include <iostream>
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"
#include "Player.hpp"
#include "ChessUI.hpp"
#include "ChessLogic.hpp"

int main()
{
	ChessLogic cl;
	cl.run_game();
	/*ChessUI ui;
	ui.init_game();
	std::string game_board[8][8] = 
		{{"WR1", "   ", "   ", "BB2", "   ", "   ", "   ", "   "},
		 {"BP1", "   ", "WKG", "   ", "   ", "   ", "   ", "   "},
		 {"   ", "WK2", "   ", "BQN", "   ", "   ", "   ", "   "},
		 {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
		 {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
		 {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
		 {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "},
		 {"   ", "   ", "   ", "   ", "   ", "   ", "   ", "   "}};
	Rook r(1, "WR1", 0, 0);
	Pawn p(-1, "BP1", 1, 0);
	Knight k(1, "WK2", 2, 1);
	Bishop b(-1, "BB2", 0, 3);
	Queen q(-1, "BQN", 2, 3);
	King kg(1, "WKG", 1, 2);
	for(const std::pair<int, int>& move : r.valid_moves(game_board))
	{
		std::cout << r.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}
	for(const std::pair<int, int>& move : p.valid_moves(game_board))
	{
		std::cout << p.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}
	for(const std::pair<int, int>& move : k.valid_moves(game_board))
	{
		std::cout << k.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}
	for(const std::pair<int, int>& move : b.valid_moves(game_board))
	{
		std::cout << b.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}
	for(const std::pair<int, int>& move : q.valid_moves(game_board))
	{
		std::cout << q.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}
	for(const std::pair<int, int>& move : kg.valid_moves(game_board))
	{
		std::cout << kg.get_string_id() << " can move to row: " << move.first << " col: " << move.second << std::endl;
	}*/
	//ui.query_row_col("what row", "what col");
	return 1;
}