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
bool Pawn::canMove(int x_dest, int y_dest, Piece* prot)
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
			return (isOpponent(side_piece) &&
					side_piece->getPieceType() == PAWN);
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
 * Return: Grid object that points to the intercepting grid
 */
Grid Pawn::interceptGrid(Piece* attacker, Piece* defender)
{
	bool isStraight;
	int att_x, att_y, def_x, def_y;
	int x_incr, y_incr, x_trv, y_trv;

	if (!attacker || !defender)
		return (Grid(-1, -1));
	att_x = attacker->getX();
	att_y = attacker->getY();
	def_x = defender->getX();
	def_y = defender->getY();

	isStraight = ((att_x - def_x) == 0) || ((att_y - def_y) == 0);

	if (isStraight)
	{
		bool y_in_bound, x_in_bound, is_forward;

		// Only consider square ahead of pawn position
		is_forward = isBlack() && (att_y < this->y) ||
			isWhite() && (att_y > this->y);

		// Checks if y position of the attacker isn't beyond reach
		y_in_bound = (abs(this->y - att_y) == 1) ||
			((abs(this->y - att_y) == 2) && !hasMoved());

		// Checks if x position is between that of the attacker and the
		// defender
		x_in_bound = (att_x < this->x && this->x < def_x) ||
			(def_x < this->x && this->x < att_x);

		if (is_forward && ((att_y - def_y) == 0) && x_in_bound && y_in_bound)
			return (Grid(this->x, att_y));
	}

	x_incr = (att_x < def_x) ? 1 : -1;
	y_incr = (att_y < def_y) ? 1 : -1;
	x_trv = y_trv = -1;

	for (x_trv = att_x, y_trv = att_y;
			x_trv != def_x && y_trv != def_y;
			x_trv += x_incr, y_trv += y_incr)
	{
		if (x_trv == this->x)
		{
			if (isBlack())
			{
				if (this->y - y_trv == 1)
				{
					if (!m_board->getPiece(x_trv, y_trv))
						return (Grid(x_trv, y_trv));
				} else if (this->y - y_trv == 2 && !hasMoved())
				{
					if (!m_board->getPiece(x_trv, y_trv) &&
							!m_board->getPiece(x_trv, y_trv + 1))
						return (Grid(x_trv, y_trv));
				}
			} else
			{
				if (y_trv - this->y == 1)
				{
					if (!m_board->getPiece(x_trv, y_trv))
						return (Grid(x_trv, y_trv));
				} else if (y_trv - this->y == 2 && !hasMoved())
				{
					if (!m_board->getPiece(x_trv, y_trv) &&
							!m_board->getPiece(x_trv, y_trv - 1))
						return (Grid(x_trv, y_trv));
				}
			}
		}

	}

	return (Grid(-1, -1));
}
