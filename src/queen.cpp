#include "../headers/pieces.h"
#include "../headers/game.h"

#define PI 3.14159265358979323846

Queen::Queen(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
        piece_type = PieceType::QUEEN;
        if (isBlack)
        {
                piece_texture = loadImage("assets/black_queen.png",
                                renderer);
        } else
        {
                piece_texture = loadImage("assets/white_queen.png",
                                renderer);
        }
}

bool Queen::canMove(int x_dest, int y_dest)
{
	Piece *tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if (((abs(x - x_dest) == 0 && abs(y - y_dest) > 0) ||
		(abs(x - x_dest) > 0 && abs(y - y_dest) == 0)) ||
		((abs(x - x_dest) > 0 && abs(y - y_dest) > 0) &&
		angle(x_dest, y_dest) == 45))
	{
		Piece* blocker;
	       
		blocker = m_board->routeBlocked(this, x_dest, y_dest);
		if (!tmp || isOpponent(tmp))
		{
			if (blocker)
				return (false);
			return (true);
		}
	}
	return (false);
}

int Queen::angle(int x_dest, int y_dest)
{
	int x_delta, y_delta;
	float a;

	x_delta = abs(x - x_dest);
	y_delta = abs(y - y_dest);
	a = atan2(static_cast<float>(y_delta), static_cast<float>(x_delta)) *
		180.0 / PI;
	return ((int) a);
}
