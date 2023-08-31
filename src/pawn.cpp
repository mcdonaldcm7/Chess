#include "../headers/pieces.h"
#include "../headers/game.h"

extern int abs(int);

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

bool Pawn::hasMoved(void) const
{
	if (is_black)
		return (y != 6);
	return (y != 1);
}

bool Pawn::canMove(int x_dest, int y_dest)
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
		if(diag_piece != nullptr) {
			return (isWhite() && diag_piece->isBlack() ||
					isBlack() && diag_piece->isWhite());
		}
	}

	//En Passant
	if(abs(x - x_dest) == 1 && ((y == 4 && y_dest == 5 && isWhite()) ||
				(y == 3 && y_dest == 2 && isBlack())))
	{
		Piece* side_piece = m_board->getPiece(x_dest, y);
		if(side_piece != nullptr) {
			if((isWhite() && side_piece->isBlack()) &&
					side_piece->getPieceType() == PAWN)
				return (true);
			else if((isBlack() && side_piece->isWhite()) &&
					side_piece->getPieceType() == PAWN)
				return (true);
		}
	}
	return (false);
}
