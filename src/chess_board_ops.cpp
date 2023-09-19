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

	// Handles peculiar like en-passant case where the piece to capture
	// isn't in the destination coordinate
	if (piece->getPieceType() == PAWN && !tmp)
	{
		tmp = m_board[x][prevY];
		m_board[x][prevY] = nullptr;
	}

	// Castling
	if (piece->getPieceType() == KING)
	{
		bool right;

		right = x > prevX;
		if (abs(prevX - x) == 2)
		{
			Rook* r;
			int rook_x;

			rook_x = right ? x + 2 : x - 1;
			r = dynamic_cast<Rook*>(m_board[rook_x][y]);
			r->setX(right ? prevX + 1 : prevX - 1);
			m_board[rook_x][y] = nullptr;
			m_board[right ? prevX + 1 : prevX - 1][y] = r;
		}
	}

	// Calling the moved function if the piece was a King or Rook
	switch (piece->getPieceType())
	{
		case KING:
			King* k;

			k = dynamic_cast<King*>(piece);
			k->moved();
			break;
		case ROOK:
			Rook* r;

			r = dynamic_cast<Rook*>(piece);
			r->moved();
			break;
		default:
			break;
	}

	if (tmp)
	{
		if (tmp->isBlack())
		{
			for (int i = 0; i < m_black_pieces.size(); i++)
			{
				if (tmp == m_black_pieces[i])
					m_black_pieces.erase(m_black_pieces.begin() + i);
			}
		} else
		{
			for (int i = 0; i < m_white_pieces.size(); i++)
			{
				if (tmp == m_white_pieces[i])
					m_white_pieces.erase(m_white_pieces.begin() + i);
			}
		}

		delete tmp;
	}

	drawBoard();
}

/**
 * trackDiagonal - Traverses each every grid diagonally leading to the
 * specified x_dest and y_dest, only advancing if no piece was found in the way
 *
 * @piece: Travelling piece
 * @x_dest: x destination of the travelling piece
 * @y_dest: y destination of the travelling piece
 *
 * Return: Pointer to the piece found in a path of piece, Otherwise nullptr
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
 * trackStraight - Traverses each and every grid in a straight line leading to
 * the specified x_dest and y_dest, only advancing if no piece was found in the
 * way
 *
 * @piece: Travelling piece
 * @x_dest: x destination of the travelling piece
 * @y_dest: y destination of the travelling piece
 *
 * Return: Pointer to the piece found in a path of piece, Otherwise nullptr
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

/**
 * routeBlocked - Checks if the path to a piece destination is blocked
 *
 * @piece: Travelling piece
 * @x_dest: x destination of the travelling piece
 * @y_dest: y destination of the travelling piece
 *
 * Return: Pointer to piece blocking the way or a nullptr if none
 */
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
		default:
			break;
	}
	return (tmp);
}

/**
 * isPieceTurn - Returns whether or not it's a piece's turn
 *
 * @piece: Piece to check
 *
 * Return: true if piece is black and it's black turn and vice versa
 */
bool ChessBoard::isPieceTurn(const Piece* piece)
{
	return ((piece->isBlack() && m_black_turn) || (piece->isWhite() && !m_black_turn));
}
