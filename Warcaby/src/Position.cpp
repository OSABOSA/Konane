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
Position<N>::Position() : boardSize(N), cursorRow(0), cursorColumn(0)
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
inline int Position<N>::selectSquare()
{
	do
	{
		showBoard();
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
//
//template<int N>
//inline auto Position<N>::findLegalPositions(const bool isWhite, board* position[3])
//{
//	std::vector<board> allLegalPositions;
//	board* legalPosition = new board[3];
//	int squareFinal, squareCaptured;
//	for (int row = 0; row < boardSize; row++)
//	{
//		for (int column = 0; column < boardSize; column++)
//		{
//			if (!position[isWhite]->test(id(row, column))) //checks is on wrong color spot 
//				continue;
//			for (int direction = 0; direction < 4; direction++)
//			{
//				legalPosition = position;
//				squareFinal		= whereMove(row, column, direction, 2);
//				squareCaptured	= whereMove(row, column, direction, 1);
//				while(squareFinal >= 0) 
//				{
//					if (position[color(empty)].test(squareFinal) && position[!isWhite].test(squareCaptured))
//					{
//						singleCapture(row, column, direction, &legalPosition);
//						allLegalPositions.push_back(&legalPosition);
//
//						squareFinal		= whereMove(r(squareFinal),		 c(squareFinal),	direction, 2);
//						squareCaptured	= whereMove(r(squareCaptured),	 c(squareCaptured), direction, 2);
//					}
//					else
//						squareFinal = -1;
//				}
//			}			
//		}
//	}
//	allLegalPositions.push_back(legalPosition);
//	return allLegalPositions;
//}

template<int N>
inline void Position<N>::singleCapture(int row, int column, const int chosenDirection, board& position)
{
	int sign = 1;
	bool direction = chosenDirection & 0b10;
	if (chosenDirection & 0b1)
		sign *= -1;
	for (int offset = 0; offset < 3; offset++)
	{
		flipSquare(row + sign * offset * direction, column + sign * !direction * offset, position); //if not possible then what?
	}

}

template<int N>
inline bool Position<N>::flipSquare(const int square)
{
	if (square == -1)
		return 0;
	piece[mask.test(square)].flip(square);
	piece[color(empty)].flip(square);
	return 1;
}

template<int N>
inline bool Position<N>::setSquare(const int square)
{
	if (square == -1)
		return 0;
	piece[mask.test(square)].set(square);
	piece[color(empty)].reset(square);
	return 1;
}

template<int N>
inline bool Position<N>::resetSquare(const int square)
{
	if (square == -1)
		return 0;
	piece[mask.test(square)].reset(square);
	piece[color(empty)].set(square);
	return 1;
}

template<int N>
inline bool Position<N>::flipSquare(const int row, const  int column)
{
	return flipSquare(id(row, column));
}

template<int N>
inline bool Position<N>::setSquare(const int row, const int column)
{
	return setSquare(id(row, column));
}

template<int N>
inline bool Position<N>::resetSquare(const int row, const int column)
{
	return resetSquare(id(row, column));
}

template<int N>
inline bool Position<N>::flipSquare(const int row, const int column, board& position)
{
	int square = id(row, column);
	if (square == -1)
		return 0;
	position[mask.test(square)].flip(square);
	position[color(empty)].flip(square);
	return 1;
}

template<int N>
inline void Position<N>::showBoard()
{
	std::tuple<uint8_t, uint8_t, uint8_t> fgCursor = std::make_tuple(255, 255, 0), bgCursor = std::make_tuple(255, 0, 0);
	std::string img;
	system("cls");
	for (int row = 0; row < boardSize; row++)
	{
		for (int column = 0; column < boardSize; column++)
		{
			if (piece[mask.test(id(row, column))][id(row, column)])
				img = " * ";
			else
				img = "   ";
			if (cursorRow == row && cursorColumn == column)
			{
				std::cout << text(img, fgCursor, bgCursor);
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
