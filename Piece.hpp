#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>
//#include <forward_list>
#include <unordered_set>
#include <utility>
#include <string>

//Base class from which all chess pieces derive from.
//ID key: black pieces = -1; blank pieces = 0; white pieces = 1

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename S, typename T> 
struct pair_hash//<std::pair<S, T>>
{
    inline std::size_t operator()(const std::pair<S, T> & v) const
    {
         std::size_t seed = 0;
         hash_combine(seed, v.first);
         hash_combine(seed, v.second);
         return seed;
    }
};

class Piece
{
	public:
		Piece(int id, std::string string_id, int row, int col) : id(id), str_id(string_id), position(std::pair<int, int>(row, col))
		{
		}

		//Return -1 if black, 0 if blank, 1 if white 
		int get_id() const
		{
			return id;
		}

		//Return -1 if white. Return 1 if black
		int get_opp_id() const
		{
			return -id;
		}

		//Return the position of this piece in terms of (row, col) pair with 0 indexing
		std::pair<int, int> get_position() const
		{
			return position;
		}

		//Return the Piece's string id, which is the string that represents the Piece on the board
		std::string get_string_id() const
		{
			return str_id;
		}

		//Set the row and column of this piece
		void set_position(int col, int row)
		{
			position.first = row;
			position.second = col;
		}

		//All pieces that derive from this Piece base class move differently, but nonetheless, they have a set of valid moves
		virtual std::unordered_set<std::pair<int, int>, pair_hash<int, int>> valid_moves(std::string game_board[][8])=0;

	protected:
		//id references what color the piece is. str_id references the name of the piece that is displayed on the board
		int id;
		std::string str_id;

		std::pair<int, int> position;
};
#endif