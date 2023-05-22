#include "../headers/Position.h"

struct not_on_board : std::exception {
	const char* what() const noexcept { return "Ooops!\n"; }
};


std::string text(std::string text, std::tuple<uint8_t, uint8_t, uint8_t> fgRGB, std::tuple<uint8_t, uint8_t, uint8_t> bgRGB)
{
	std::string out = "";
	out.append("\033[38;2");
	out.append(";");
	out.append(std::to_string(std::get<0>(fgRGB)));
	out.append(";");
	out.append(std::to_string(std::get<1>(fgRGB)));
	out.append(";");
	out.append(std::to_string(std::get<2>(fgRGB)));
	out.append(";48;2");
	out.append(";");
	out.append(std::to_string(std::get<0>(bgRGB)));
	out.append(";");
	out.append(std::to_string(std::get<1>(bgRGB)));
	out.append(";");
	out.append(std::to_string(std::get<2>(bgRGB)));
	out.append("m");
	out.append(text);
	out.append("\033[0m");
	return out;
}

int getArrowKeysMovement()
{
	int arrow;
	do
	{
		arrow = _getch();
		if (arrow == 224)
			arrow = _getch();
	} while (!(arrow == 72 || arrow == 75 || arrow == 77 || arrow == 80 || arrow == 13));
	return arrow;
}

template<int N>
Position<N>::Position() : boardSize(N), cursorRow(0), cursorColumn(0), pieceJumpingRow(-1), pieceJumpingColumn(-1), pieceLandingRow(-1), pieceLandingColumn(-1)
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t square = 0; square < piece[color(empty)].size(); square++)
		{
			piece[i].set(square, (square + (int(square / N) % 2)) % 2);
		}
	}
	piece[color(black)].flip();
	piece[color(empty)].reset();
	mask = piece[color(white)];
}

template<int N>
inline int Position<N>::selectSquareID()
{
	do
	{
		showBoard(piece);//it can be piece coz it doesnt change it
	} while (moveCursor(cursorRow, cursorColumn) >= 0);
	return id(cursorRow, cursorColumn);
}

template<int N>
inline int Position<N>::moveCursor(int row, int column)
{
	int arrow = getArrowKeysMovement();
	if (arrow == 75)
		--column;
	else if (arrow == 72)
		--row;
	else if (arrow == 77)
		++column;
	else if (arrow == 80)
		++row;
	else
		return -1;
	if (id(row, column) >= 0)
	{
		cursorRow = row;
		cursorColumn = column;
		return 1;
	}
	return 0;
}

template<int N>
inline int Position<N>::whereMove(int row, int column, const int direction, int length)
{
	if (direction & 0b1)
		length *= -1;
	if (direction & 0b10)
		column += length;
	else
		row += length;

	return id(row, column);
}

template<int N>
inline int Position<N>::whereMove(int squareID, const int direction, int length)
{
	return whereMove(r(squareID), c(squareID), direction, length);
}

template<int N>
inline void Position<N>::singleCaptureByDirection(int row, int column, const int chosenDirection, board* position)
{
	int sign = 1;
	bool direction = chosenDirection & 0b10;
	if (chosenDirection & 0b1)
		sign *= -1;
	for (int offset = 0; offset < 3; offset++)
	{
		flipSquare(row + sign * offset * !direction, column + sign * direction * offset, position); //if not possible then what?
	}

}

template<int N>
void Position<N>::singleCaptureByDirection(int squareID, const int direction, board* position)
{
	singleCaptureByDirection(r(squareID), c(squareID), direction, position);
}

template<int N>
void Position<N>::singleCaptureByPosition(int squareCurrentID, int squareCapturedID, int squareFinalID, board* position)
{
	flipSquare(squareCurrentID, position);
	flipSquare(squareCapturedID, position);
	flipSquare(squareFinalID, position);
}

template<int N>
inline bool Position<N>::flipSquare(const int row, const int column, board* position)
{
	return flipSquare(id(row, column), position);
}

template<int N>
bool Position<N>::flipSquare(const int squareID, board* position)
{
	if (squareID == -1)
		return 0;
	position[mask.test(squareID)].flip(squareID);
	position[color(empty)].flip(squareID);
	return 1;
}

template<int N>
inline void Position<N>::showBoard(board* position)
{
	std::tuple<uint8_t, uint8_t, uint8_t> fgCursor = std::make_tuple(255, 255, 0), bgCursor = std::make_tuple(255, 0, 0);
	std::tuple<uint8_t, uint8_t, uint8_t> fgSelected = std::make_tuple(155, 0, 155), bgSelected = std::make_tuple(100, 255, 100);
	std::string img;
	system("cls");
	for (int row = 0; row < boardSize; row++)
	{
		for (int column = 0; column < boardSize; column++)
		{
			if (position[mask.test(id(row, column))][id(row, column)])
				img = " * ";
			else
				img = "   ";
			if (cursorRow == row && cursorColumn == column)
			{
				std::cout << text(img, fgCursor, bgCursor);
				continue;
			}
			if (pieceJumpingRow == row && pieceJumpingColumn == column)
			{
				std::cout << text(img, fgSelected, bgSelected);
				continue;
			}
			if (mask.test(id(row, column)))
				std::cout << text(img, std::make_tuple(255, 255, 255), std::make_tuple(60, 60, 60));
			else
				std::cout << text(img, std::make_tuple(20, 20, 20), std::make_tuple(200, 200, 200));
		}
		std::cout << std::endl;
	}
}

template<int N>
void Position<N>::playerMove(const bool isWhite, board* position)
{

	do
	{
		changeJumpingSquare(isWhite, position);
		changeLandingSquare(isWhite, position);
	} while (id(pieceJumpingRow, pieceJumpingColumn) == -1);
	capture(isWhite, pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn, position);
	resetPieceJumping();
	showBoard(position);
}

template<int N>
void Position<N>::changeJumpingSquare(bool isWhite, board* position) //why does it fuckup
{
	int squareID;
	do
	{
		squareID = selectSquareID();
	} while (!canPieceCapture(isWhite, squareID, position));
	if (squareID == id(pieceJumpingRow, pieceJumpingColumn))
	{
		pieceJumpingRow = -1;
		pieceJumpingColumn = -1;
	}
	else
	{
		pieceJumpingRow = r(squareID);
		pieceJumpingColumn = c(squareID);
	}
}

template<int N>
void Position<N>::changeLandingSquare(bool isWhite, board* position)
{
	int squareID;
	do{
		squareID = selectSquareID();
		if (squareID == id(pieceJumpingRow, pieceJumpingColumn))
		{
			resetPieceJumping();
			resetPieceLanding();
			break;
		}
		if (canPieceCapture(isWhite, squareID, position))
		{
			setPieceJumping(squareID);
			continue;
		}
		setPieceLanding(squareID);
	} while (!isCaptureLegal(isWhite, pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn, position));
}

template<int N>
bool Position<N>::isPiece(bool isWhite, int squareID, board* position)
{
	return position[isWhite].test(squareID);
}

template<int N>
void Position<N>::removePiece(bool isWhite, board* position)
{
	int squareID;
	do
	{
		squareID = selectSquareID();
	} while (!isPiece(isWhite, squareID, position));
	flipSquare(squareID, position);
	showBoard(position);
}

template<int N>
bool Position<N>::canPieceCaptureInDirection(bool isWhite, int squareID, int direction, board* position)
{
	int squareFinal = whereMove(r(squareID), c(squareID), direction, 2);
	int squareCaptured = whereMove(r(squareID), c(squareID), direction, 1);
	int squareCurrent = squareID;
	if (squareFinal < 0)
		return 0;
	if (isSingleCaptureLegal(isWhite, squareCurrent, squareCaptured, squareFinal, position))
		return 1;
	return 0;
}

template<int N>
bool Position<N>::canPieceCapture(bool isWhite, int squareID, board* position)
{
	for (int direction = 0; direction < 4; direction++)
	{
		if (canPieceCaptureInDirection(isWhite, squareID, direction, position))
			return 1;
	}
	return 0;
}

template<int N>
int Position<N>::whichDirection(int startRow, int startColumn, int endRow, int endColumn)
{
	int direction = 0;
	if (endRow - startRow < 0 || endColumn - startColumn < 0) //not safe if diagonal
		direction += 0b1;
	if (endRow == startRow)
		direction += 0b10;
	return direction;
}

template<int N>
void Position<N>::resetPieceJumping()
{
	setPieceJumping(-1, -1);
}

template<int N>
void Position<N>::resetPieceLanding()
{
	setPieceLanding(-1, -1);
}

template<int N>
void Position<N>::setPieceJumping(int row, int column)
{
	pieceJumpingRow = row;
	pieceJumpingColumn = column;
}

template<int N>
void Position<N>::setPieceJumping(int squareID)
{
	setPieceJumping(r(squareID), c(squareID));
}

template<int N>
void Position<N>::setPieceLanding(int row, int column)
{
	pieceLandingRow = row;
	pieceLandingColumn = column;
}

template<int N>
void Position<N>::setPieceLanding(int squareID)
{
	setPieceLanding(r(squareID), c(squareID));
}

template<int N>
void Position<N>::capture(const bool isWhite, int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn, board* position)
{
	int direction = whichDirection(pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn);
	int squareID = id(pieceJumpingRow, pieceJumpingColumn);
	while (squareID != id(pieceLandingRow, pieceLandingColumn))//wrong statement
	{
		singleCaptureByDirection(squareID, direction, position);//has to be multiple capture
		squareID = whereMove(squareID, direction, 2);
	}
}

template<int N>
bool Position<N>::isMoveCaptureAlike(int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn)
{
	if (pieceJumpingRow != pieceLandingRow && pieceJumpingColumn != pieceLandingColumn)
		return false;
	if (mask.test(id(pieceJumpingRow, pieceJumpingColumn)) != mask.test(id(pieceLandingRow, pieceLandingColumn)))
		return false;
	return true;
}

template<int N>
bool Position<N>::isSingleCaptureLegal(const bool isWhite, int squareCurrentID, int squareCapturedID, int squareFinalID, board* position)
{
	return isSingleCaptureLegal(isWhite, squareCurrentID, squareCapturedID, squareFinalID, position, 0);
}

template<int N>
bool Position<N>::isSingleCaptureLegal(const bool isWhite, int squareCurrentID, int squareCapturedID, int squareFinalID, board* position, bool omitCurrentSquare)
{
	return ((omitCurrentSquare || position[isWhite].test(squareCurrentID)) && position[!isWhite].test(squareCapturedID) && position[color(empty)].test(squareFinalID));
}

template<int N>
bool Position<N>::isCaptureLegal(const bool isWhite, int pieceJumpingRow, int pieceJumpingColumn, int pieceLandingRow, int pieceLandingColumn, board* position)
{
	int squareID = id(pieceJumpingRow, pieceJumpingColumn);
	int direction = whichDirection(pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn);
	if (!isMoveCaptureAlike(pieceJumpingRow, pieceJumpingColumn, pieceLandingRow, pieceLandingColumn))
		return false;
	for (int i = 0; i < abs(pieceLandingRow+pieceLandingColumn-pieceJumpingRow-pieceJumpingColumn); i+=2)
	{
		if (!isSingleCaptureLegal(isWhite, squareID, whereMove(squareID, direction, 1), whereMove(squareID, direction, 2), position, i))
			return false;
		squareID = whereMove(squareID, direction, 2);
	}
	return true;
}

template<int N>
inline int Position<N>::id(const int row, const int column)const
{
	if (row >= N || row < 0)
		return -1;
	if (column >= N || column < 0)
		return -1;
	return row * N + column;
}

template<int N>
inline int Position<N>::r(const int squareID)
{
	return squareID / N;
}

template<int N>
inline int Position<N>::c(const int squareID)
{
	return squareID % N;
}

template<int N>
inline auto Position<N>::findLegalPositions(const bool isWhite, board* position)
{
	
	std::vector<board*> allLegalPositions;
	int squareFinal, squareCaptured, squareCurrent;
	for (int row = 0; row < boardSize; row++)
	{
		for (int column = 0; column < boardSize; column++)
		{
			if (!position[isWhite].test(id(row, column))) //checks if on wrong color spot 
				continue;
			for (int direction = 0; direction < 4; direction++)
			{
				board* legalPosition = new board[3];
				std::copy(position, position + 3, legalPosition);
				squareFinal		= whereMove(row, column, direction, 2);
				squareCaptured	= whereMove(row, column, direction, 1);
				squareCurrent = id(row, column);
				while(squareFinal >= 0) 
				{
					if (isSingleCaptureLegal(isWhite, squareCurrent, squareCaptured, squareFinal, position))
					{
						singleCaptureByPosition(squareCurrent, squareCaptured, squareFinal, legalPosition);
						allLegalPositions.push_back(legalPosition); // why does it change piece

						squareFinal		= whereMove(squareFinal,	direction, 2);
						squareCaptured	= whereMove(squareCaptured, direction, 2);
						squareCurrent	= whereMove(squareCurrent,	direction, 2);
					}
					else
						squareFinal = -1;
				}
				delete[] legalPosition;
			}			
		}
	}
	return allLegalPositions;
}

template<int N>
bool Position<N>::noMoves(const bool isWhite, board* position)
{
	return findLegalPositions(isWhite, position).empty();
}
