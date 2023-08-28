#include "../headers/pieces.h"

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
}

bool King::canMove(int x, int y)
{
	// Moving algorithm for king
	return (false);
}
