#include "../headers/pieces.h"
#include "../headers/game.h"

Knight::Knight(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::KNIGHT;
	if (isBlack)
	{
		piece_texture = (this->x == 1) ? loadImage(
				"assets/black_knight.png", renderer) :
			loadImage("assets/black_knight2.png", renderer);
	} else
	{
		piece_texture = (this->x == 1) ? loadImage(
				"assets/white_knight.png", renderer) :
			loadImage("assets/white_knight2.png", renderer);
	}
}

/**
 * canMove - Checks whether or not the knight piece moving to the grid
 * specified by (x_dest, y_dest) is valid
 *
 * @x_dest: x-axis destination
 * @y_dest: y_axis destonation
 * @nullptr: Piece to ignore when checking for valid moves
 *
 * Return: true if a valid move was requested, false otherwise
 */
bool Knight::canMove(int x_dest, int y_dest, Piece* prot = nullptr)
{
	Piece* tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if ((abs(y_dest - y) == 2 && abs(x_dest - x) == 1) ||
			abs(x_dest - x) == 2 && abs(y_dest - y) == 1)
	{
		if (!tmp || (tmp && isOpponent(tmp)) || tmp == prot)
			return (true);
	}
	return (false);
}
