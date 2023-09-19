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
 * @prot: Piece to ignore when checking for valid moves
 *
 * Return: true if a valid move was requested, false otherwise
 */
bool Knight::canMove(int x_dest, int y_dest, Piece* prot)
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

/**
 * interceptGrids - Finds and return the intercept grid in the route from the
 * attacker to the defender (typically the king)
 *
 * @attacker: Pointer to the attacking piece
 * @defender: Pointer to the defending piece
 *
 * Return: vector of Grid object(s) that points to the intercepting grid(s)
 */
std::vector<Grid> Knight::interceptGrids(Piece* attacker, Piece* defender)
{
	std::vector<Grid> grids;
	int att_x, att_y, def_x, def_y, x_incr, y_incr, diff;

	if (!attacker || !defender)
		return (grids);
	att_x = attacker->getX();
	att_y = attacker->getY();
	def_x = defender->getX();
	def_y = defender->getY();

	if (att_x < def_x)
		x_incr = 1;
	else if (att_x > def_x)
		x_incr = -1;
	else
		x_incr = 0;

	if (att_y < def_y)
		y_incr = 1;
	else if (att_y > def_y)
		y_incr = -1;
	else
		y_incr = 0;

	for (int x_trv = att_x, y_trv = att_y; (x_trv != def_x) ||
			(y_trv != def_y); x_trv += x_incr, y_trv += y_incr)
	{
		if ((abs(y_trv - this->y) == 2 && abs(x_trv - this->x) == 1) ||
				abs(x_trv - this->x) == 2 && abs(y_trv - this->y) == 1)
			grids.push_back(Grid(x_trv, y_trv));
	}

	return (grids);
}
