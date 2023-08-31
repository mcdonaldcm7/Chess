#include "../headers/game.h"
#include "../headers/pieces.h"

/**
 * movePiece - Updates the position of piece, and m_board with the specified
 * position
 *
 * @piece: Piece object being moved
 * @x: x position to move piece to
 * @y: y position to move piece to
 *
 * Return: Nothing
 */
void ChessBoard::movePiece(Piece* piece, int x, int y)
{
	int prevX, prevY;
	Piece* tmp;

	prevX = piece->getX();
	prevY = piece->getY();
	tmp = m_board[x][y];

	m_board[x][y] = piece;
	piece->setX(x);
	piece->setY(y);
	m_board[prevX][prevY] = nullptr;

	// Handles peculiar like en-passant case where thee piece to capture
	// isn't in the destination coordinate
	if (piece->getPieceType() == PAWN && !tmp)
	{
		tmp = m_board[x][prevY];
		m_board[x][prevY] = nullptr;
	}
	if (tmp)
		delete tmp;
	drawBoard();
}

/**
 * trackDiagonal - Traverses diagonally (X motion i.e Bishop) to the specified
 * destination in search of a piece along the path to the destination.
 *
 * @piece: Travelling piece
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 *
 * Return: Obstructing piece or nullptr if no piece was along the path
 */
Piece* ChessBoard::trackDiagonal(Piece* piece, int x_dest, int y_dest)
{
	Piece* blocker;
	bool upper, left;
	int x_r, y_r;

	upper = y_dest < piece->getY();
	left = x_dest < piece->getX();

	x_r = left ? piece->getX() - 1 : piece->getX() + 1;
	y_r = upper ? piece->getY() - 1 : piece->getY() + 1;

	blocker = nullptr;

	for (; abs(x_r - x_dest) != 0 && abs(y_r - y_dest) != 0;
			x_r += left ? -1 : 1, y_r += upper ? -1 : 1)
	{
		blocker = m_board[x_r][y_r];
		if (blocker != nullptr)
			break;
	}
	return (blocker);
}

/**
 * trackStraight - Traverses a straight path (Horizontally and Vertically e.g
 * Rook) to detect obstructions along the path.
 *
 * @piece: Travelling piece
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 *
 * Return: Obstructing piece or nullptr if no piece was along the path
 */
Piece* ChessBoard::trackStraight(Piece* piece, int x_dest, int y_dest)
{
	Piece* blocker;

	blocker = nullptr;
	if (abs(piece->getX() - x_dest) > 0)
	{
		bool left;
		int x_r;

		left = x_dest < piece->getX();
		x_r = left ? piece->getX() - 1 : piece->getX() + 1;
		for (; abs(x_r - x_dest) != 0; x_r += left ? -1 : 1)
		{
			blocker = m_board[x_r][y_dest];
			if (blocker != nullptr)
				break;
		}
	} else
	{
		bool down;
		int y_r;

		down = y_dest > piece->getY();
		y_r = down ? piece->getY() + 1 : piece->getY() - 1;
		for (; abs(y_r - y_dest) != 0; y_r += down ? 1 : -1)
		{
			blocker = m_board[x_dest][y_r];
			if (blocker != nullptr)
				break;
		}
	}

	return (blocker);
}

Piece* ChessBoard::routeBlocked(Piece* piece, int x_dest, int y_dest)
{
	Piece* tmp;
	if (!piece)
		return (nullptr);

	tmp = nullptr;
	switch(piece->getPieceType())
	{
		case PAWN:
			Piece* piece1, *piece2;

			piece1 = m_board[x_dest][y_dest];
			// Using +1 and -1 cause depending on the side it moves
			// forward or backward and we want the prior piece
			piece2 = piece->isBlack() ? m_board[x_dest][y_dest + 1]
				: m_board[x_dest][y_dest - 1];

			tmp = piece1 != nullptr ? piece1 : piece2;
			break;
		case BISHOP:
			tmp = trackDiagonal(piece, x_dest, y_dest);
			break;
		case ROOK:
			tmp = trackStraight(piece, x_dest, y_dest);
			break;
		case QUEEN:
			bool moving_diagonally;

			moving_diagonally = abs(piece->getX() - x_dest) > 0 && abs(piece->getY() - y_dest) > 0;

			if (moving_diagonally)
				tmp = trackDiagonal(piece, x_dest, y_dest);
			else
				tmp = trackStraight(piece, x_dest, y_dest);
			break;
		case KING:
			break;
		default:
			tmp = nullptr;
			break;
	}
	return (tmp);
}
