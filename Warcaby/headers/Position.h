#pragma once
#include "Includes.h"


template <int N>
class Position
{
public:
	Position();
	using board = std::bitset<N* N>;
	board piece[3];//[black, white, empty]
	board mask;
	int cursorRow, cursorColumn;
	int selectSquare();
	int moveCursor(int row, int column);
	int whereMove(int row, int column, const int direction, int length);
	void singleCapture(int row, int column, const int direction, board& position);
	bool flipSquare(const int squareID);
	bool setSquare(const int squareID);
	bool resetSquare(const int squareID);
	bool flipSquare(const int row, const int column);
	bool setSquare(const int row, const int column);
	bool resetSquare(const int row, const int column);
	bool flipSquare(const int row, const int column, board& position);
	void showBoard();
	enum color
	{
		black = 0,
		white = 1,
		empty = 2
	};
	enum direction
	{
		up		= 0b01,
		down	= 0b00,
		left	= 0b11,
		right	= 0b10
	};
private:
	int id(const int row, const int column) const;
	int r(const int squareID);
	int c(const int squareID);
	const uint8_t boardSize;
};

#include "../src/Position.cpp"
