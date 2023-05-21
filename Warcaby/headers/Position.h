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
	int cursorRow, cursorColumn, pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn;
	int selectSquareID();
	int moveCursor(int row, int column);
	int whereMove(int row, int column, const int direction, int length);
	int whereMove(int squareID, const int direction, int length);
	void singleCaptureByDirection(int row, int column, const int direction, board* position);
	void singleCaptureByDirection(int squareID, const int direction, board* position);
	void singleCaptureByPosition(int squareCurrentID, int squareCapturedID, int squareFinalID, board* position);
	bool flipSquare(const int row, const int column, board* position);
	bool flipSquare(const int squareID, board* position);
	auto findLegalPositions(const bool isWhite, board* position);
	bool noMoves(const bool isWhite, board* position);
	void showBoard(board* position);
	void playerMove(const bool isWhite, board* position);
	void changeJumpingSquare(const bool isWhite, board* position); 
	void changeLandingSquare(bool isWhite, board* position);//name doesnt strictly correspond to function
	bool isPiece(bool isWhite, int squareID, board* position);
	void removePiece(bool isWhite, board* position);
	bool canPieceCaptureInDirection(bool isWhite, int squareID, int direction, board* position);
	bool canPieceCapture(bool isWhite, int squareID, board* position);
	int whichDirection(int startRow, int startColumn, int endRow, int endColumn);
	void resetPieceJumping();
	void resetPieceLanding();
	void setPieceJumping(int row, int column);
	void setPieceJumping(int squareID);
	void setPieceLanding(int row, int column);
	void setPieceLanding(int squareID);
	void capture(const bool isWhite, int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn, board* position);
	bool isMoveCaptureAlike(int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn);
	bool isSingleCaptureLegal(const bool isWhite, int squareCurrentID, int squareCapturedID, int squareFinalID, board* position);
	bool isSingleCaptureLegal(const bool isWhite, int squareCurrentID, int squareCapturedID, int squareFinalID, board* position, bool omitCurrentSquare);
	bool isCaptureLegal(const bool isWhite, int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn, board* position);
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


	int id(const int row, const int column) const;
	int r(const int squareID);
	int c(const int squareID);
	const uint8_t boardSize;
private:
	
};

#include "../src/Position.cpp"
