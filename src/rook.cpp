#include "../headers/pieces.h"
#include "../headers/game.h"

Rook::Rook(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::ROOK;
	if (isBlack)
	{
		piece_texture = loadImage("assets/black_rook.png",
				renderer);
	} else
	{
		piece_texture = loadImage("assets/white_rook.png",
				renderer);
	}
	m_has_moved = false;
}

/**
 * canMove - Checks whether or not the rook is allowed to move to the grid
 * specified by (x_dest, y_dest)
 *
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 * @prot: Piece to ignore when performing checks
 *
 * Return: true if requested move is valid, false otherwise
 */
bool Rook::canMove(int x_dest, int y_dest, Piece* prot = nullptr)
{
	Piece* tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if (abs(x - x_dest) == 0 && abs(y - y_dest) > 0 ||
			abs(x - x_dest) > 0 && abs(y - y_dest) == 0)
	{
		// Moving Vertically
		// Consider case where piece is defending the king
		if (!tmp || isOpponent(tmp) || (tmp == prot))
		{
			Piece* blocker;

			blocker = m_board->routeBlocked(this, x_dest, y_dest);
			if (blocker)
				return (false);
			return (true);
		}
	}
	return (false);
}
