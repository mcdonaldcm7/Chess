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

/**
 * canMove - Checks whether or not the queen is allowed to move to the grid
 * specified by (x_dest, y_dest)
 *
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 * @nullptr: Piece to ignore when performing checks
 *
 * Return: true if requested move is valid, false otherwise
 */
bool Queen::canMove(int x_dest, int y_dest, Piece* prot = nullptr)
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
		if (!tmp || isOpponent(tmp) || (tmp == prot))
		{
			if (blocker && (blocker != prot))
				return (false);
			return (true);
		}
	}
	return (false);
}

/**
 * angle - Computes and returns the angle of the queen piece relative to the
 * specified destination
 *
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 *
 * Return: Angle of the queen piece relative to the destination
 */
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
