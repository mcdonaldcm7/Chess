#include "../headers/pieces.h"
#include "../headers/game.h"

King::King(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::KING;
	if (isBlack)
	{
		piece_texture = loadImage("assets/black_king.png",
				renderer);
	} else
	{
		piece_texture = loadImage("assets/white_king.png",
				renderer);
	}
	m_has_moved = false;
	m_on_check = false;
	m_attacker = nullptr;
}

bool King::canMove(int x_dest, int y_dest, Piece* prot = nullptr)
{
	Piece* tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if (abs(x - x_dest) <= 1 && abs(y - y_dest) <= 1)
	{
		if (!tmp)
		{
			if (m_board->isSafe(this, x_dest, y_dest))
				return (true);
		} else
		{
			if ((isOpponent(tmp) && !tmp->isCovered()) || tmp == prot)
				return (true);
		}
	}

	// Castling
	if (abs(y - y_dest) == 0 && abs(x - x_dest) == 2 && !m_has_moved)
	{
		Piece* p;
		bool right;

		right = x_dest > x;
		if (right)
		{
			if (m_board->getPiece(x + 1, y) || m_board->getPiece(x + 2, y))
				return (false);
		} else
		{
			if (m_board->getPiece(x - 1, y) || m_board->getPiece(x - 2, y)
					|| m_board->getPiece(x - 3, y))
				return (false);
		}
		p = m_board->getPiece(right ? x_dest + 1 : x_dest - 2, y);
		if (p && p->getPieceType() == ROOK)
		{
			// Add threats check.
			// Castling should not be possible if the king passes
			// through check or if castling will place him on check
			Rook* r;

			r = dynamic_cast<Rook*>(p);
			if (!r->hasMoved())
				return (true);
		}
	}
	return (false);
}
