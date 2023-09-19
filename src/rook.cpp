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
bool Rook::canMove(int x_dest, int y_dest, Piece* prot)
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
			if (blocker && (blocker != prot))
				return (false);
			return (true);
		}
	}
	return (false);
}

/**
 * interceptGrid - Finds and return the intercept grid in the route from the
 * attacker to the defender (typically the king)
 *
 * @attacker: Pointer to the attacking piece
 * @defender: Pointer to the defending piece
 *
 * Return: Grid object that points to the intercepting grid(s)
 */
Grid Rook::interceptGrid(Piece* attacker, Piece* defender)
{
	std::vector<Grid> grids;
	bool isStraight;
	int att_x, att_y, def_x, def_y;

	if (!attacker || !defender)
		return (Grid(-1, -1));
	att_x = attacker->getX();
	att_y = attacker->getY();
	def_x = defender->getX();
	def_y = defender->getY();

	isStraight = ((att_x - def_x) == 0) || ((att_y - def_y) == 0);
	if (isStraight)
	{
		bool y_in_bound, x_in_bound;

		// Checks if y position is between that of the attacker and the
		// defender
		y_in_bound = (att_y < this->y && this->y < def_y) ||
			(def_y < this->y && this->y < att_y);

		// Checks if x position is between that of the attacker and the
		// defender
		x_in_bound = (att_x < this->x && this->x < def_x) ||
			(def_x < this->x && this->x < att_x);

		if ((att_x - def_x) == 0 && y_in_bound)
			return (Grid(att_x, this->y));
		else if ((att_y - def_y) == 0 && x_in_bound)
			return (Grid(this->x, att_y));
	} else
	{
		int abs_x, abs_y, x_incr, y_incr;

		x_incr = (att_x < def_x) ? 1 : -1;
		y_incr = (att_y < def_y) ? 1 : -1;
		abs_x = abs(att_x - def_x);
		abs_y = abs(att_y - def_y);

		for (int x_trv = att_x, y_trv = att_y;
				x_trv != def_x && y_trv != def_y;
				x_trv += x_incr, y_trv += y_incr)
		{
			if (x_trv == this->x || y_trv == this->y)
			{
				if (canMove(x_trv, y_trv))
					return (Grid(x_trv, y_trv));
			}
		}
	}
	return (Grid(-1, -1));
}
