#include "../headers/pieces.h"
#include "../headers/game.h"

#define PI 3.14159265358979323846

Bishop::Bishop(int x, int y, bool isBlack, SDL_Renderer* renderer,
		ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::BISHOP;
	if (isBlack)
	{
		piece_texture = loadImage("assets/black_bishop.png",
				renderer);
	} else
	{
		piece_texture = loadImage("assets/white_bishop.png",
				renderer);
	}
}

/**
 * canMove - Checks if the bishop piece can move to the position specified by
 * x_dest and y_dest
 *
 * @x_dest: x-axis destination
 * @y_dest: y-axis destination
 * @prot: Optional argument to help detect protected pieces
 *
 * Return: true if bishop can move to destination, Otherwise return false
 */
bool Bishop::canMove(int x_dest, int y_dest, Piece* prot)
{
	Piece* tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if (abs(x - x_dest) > 0 && abs(y - y_dest) > 0 &&
			angle(x_dest, y_dest) == 45)
	{
		Piece* blocker;

		blocker = m_board->routeBlocked(this, x_dest, y_dest);

		if (blocker && (blocker != prot))
			return (false);
		if (!tmp || isOpponent(tmp) || (tmp == prot))
			return (true);
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
int Bishop::angle(int x_dest, int y_dest)
{
	int x_delta, y_delta;
	float a;

	x_delta = abs(x - x_dest);
	y_delta = abs(y - y_dest);
	a = atan2(static_cast<float>(y_delta), static_cast<float>(x_delta)) *
		180.0 / PI;
	return ((int) a);
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
std::vector<Grid> Bishop::interceptGrids(Piece* attacker, Piece* defender)
{
	std::vector<Grid> grids;
	bool isStraight;
	int att_x, att_y, def_x, def_y, x_incr, y_incr;

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
		int x_diff, y_diff;

		x_diff = abs(this->x - x_trv);
		y_diff = abs(this->y - y_trv);
		if ((x_diff + y_diff) % 2 == 0)
		{
			if (canMove(x_trv, y_trv))
				grids.push_back(Grid(x_trv, y_trv));
		}
	}

	return (grids);
}
