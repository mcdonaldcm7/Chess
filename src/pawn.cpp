#include "../headers/pieces.h"
#include "../headers/game.h"

Pawn::Pawn(int x, int y, bool isBlack, SDL_Renderer* renderer,
		ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::PAWN;
	if (isBlack)
	{
		piece_texture = loadImage("assets/black_pawn.png",
				renderer);
	} else
	{
		piece_texture = loadImage("assets/white_pawn.png",
				renderer);
	}
}

/**
 * hasMoved - Checks whether or not the pawn has moved
 *
 * Return: true if piece has moved, false otherwise
 */
bool Pawn::hasMoved(void) const
{
	if (is_black)
		return (y != 6);
	return (y != 1);
}

/**
 * canMove - Checks whether or not the pawn is allowed to move to the grid
 * specified by (x_dest, y_dest)
 *
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 * @prot: Piece to ignore when performing checks
 *
 * Return: true if requested move is valid, false otherwise
 */
bool Pawn::canMove(int x_dest, int y_dest, Piece* prot = nullptr)
{
	// Pawns can move up to two squares forward on their first move and
	// only one square forward afterwards. They may also capture one square
	// diagonally.
	// Note: Pawns can only move toward their opponent side of the board

	// Handles movement one up for black (Only forward)

	bool is_forward, is_straight;

	is_forward = (isWhite() && (y - y_dest) < 0) || (isBlack() &&
		(y - y_dest) > 0);
	is_straight = (x - x_dest) == 0;


	// If movement is not forward, Quit!!!
	if (!is_forward)
		return (false);

	// One-Square Advance
	if (abs(y - y_dest) == 1 && is_straight &&
			m_board->getPiece(x_dest, y_dest) == nullptr)
	{
		return (true);
	}

	// Two-Square Advance
	if(abs(y - y_dest) == 2 && is_straight && !hasMoved() &&
			!m_board->routeBlocked(this, x_dest, y_dest))
	{
		return (true);
	}

	//Capture move on either forward diagonals
	if(abs(x - x_dest) == 1 && abs(y - y_dest) == 1)
	{
		Piece* diag_piece = m_board->getPiece(x_dest, y_dest);
		if (diag_piece != nullptr) {
			return (isOpponent(diag_piece) || diag_piece == prot);
		}
	}

	//En Passant
	if(abs(x - x_dest) == 1 && ((y == 4 && y_dest == 5 && isWhite()) ||
				(y == 3 && y_dest == 2 && isBlack())))
	{
		Piece* side_piece = m_board->getPiece(x_dest, y);
		if(side_piece != nullptr)
			return (isOpponent(side_piece) && side_piece->getPieceType() == PAWN);
	}
	return (false);
}
