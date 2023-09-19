#include "../headers/game.h"
#include "../headers/pieces.h"

/**
 * highlightKingRoutes - Renders the appropriate highlight for the selected
 * king piece
 *
 * @p: Pointer to the selected king piece
 *
 * Return: Nothing
 */
void ChessBoard::highlightKingRoutes(Piece* p)
{
	Piece* tmp;
	int x, y;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();

	// Highlights the square just UP of the king
	if (y <= 6)
	{
		tmp = m_board[x][y + 1];
		if (isSafe(p, x, y + 1))
		{
			if (!tmp)
				highlight(x, y + 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x, y + 1, CAPTURE);
		}
	}

	// Highlights the square just DOWN of the king
	if (y >= 1)
	{
		tmp = m_board[x][y - 1];
		if (isSafe(p, x, y - 1))
		{
			if (!tmp)
				highlight(x, y - 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x, y - 1, CAPTURE);
		}
	}

	// Highlights the RIGHT square of the king
	if (x <= 6)
	{
		tmp = m_board[x + 1][y];
		if (isSafe(p, x + 1, y))
		{
			if (!tmp)
				highlight(x + 1, y, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x + 1, y, CAPTURE);
		}
	}

	// Highlights the LEFT square of the king
	if (x >= 1)
	{
		tmp = m_board[x - 1][y];
		if (isSafe(p, x - 1, y))
		{
			if (!tmp)
				highlight(x - 1, y, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x - 1, y, CAPTURE);
		}
	}

	// Highlights the UPPER-RIGHT grid of the king
	if (y <= 6 && x <= 6)
	{
		tmp = m_board[x + 1][y + 1];
		if (isSafe(p, x + 1, y + 1))
		{
			if (!tmp)
				highlight(x + 1, y + 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x + 1, y + 1, CAPTURE);
		}
	}

	// Highlights the LOWER-RIGHT grid of the king
	if (y >= 1 && x <= 6)
	{
		tmp = m_board[x + 1][y - 1];
		if (isSafe(p, x + 1, y - 1))
		{
			if (!tmp)
				highlight(x + 1, y - 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x + 1, y - 1, CAPTURE);
		}
	}

	// Highlights the UPPER-LEFT grid of the king
	if (y <= 6 && x >= 1)
	{
		tmp = m_board[x - 1][y + 1];
		if (isSafe(p, x - 1, y + 1))
		{
			if (!tmp)
				highlight(x - 1, y + 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x - 1, y + 1, CAPTURE);
		}
	}

	// Highlights the LOWER-LEFT grid of the king
	if (y >= 1 && x >= 1)
	{
		tmp = m_board[x - 1][y - 1];
		if (isSafe(p, x - 1, y - 1))
		{
			if (!tmp)
				highlight(x - 1, y - 1, MOVE);
			else if (p->isOpponent(tmp) && !tmp->isCovered())
				highlight(x - 1, y - 1, CAPTURE);
		}
	}

	// Castling
	King* k;

	k = dynamic_cast<King*>(p);
	if (!k->hasMoved())
	{
		Piece* tmp2;
		Rook* tmp_r;

		tmp = m_board[7][y];
		tmp2 = m_board[0][y];
		if (!trackStraight(p, 7, y) && tmp->getPieceType() == ROOK)
		{
			tmp_r = dynamic_cast<Rook*>(tmp);
			if (!tmp_r->hasMoved())
				highlight(5, y, CASTLING);
		}

		if (!trackStraight(p, 0, y) && tmp2->getPieceType() == ROOK)
		{
			tmp_r = dynamic_cast<Rook*>(tmp2);
			if (!tmp_r->hasMoved())
				highlight(1, y, CASTLING);
		}
	}
}

/**
 * highlightPawnRoutes - Renders the appropriate highlight for the selected
 * pawn piece
 *
 * @p: Pointer to the selected pawn piece
 *
 * Return: Nothing
 */
void ChessBoard::highlightPawnRoutes(Piece* p)
{
	Pawn* piece;
	Piece* tmp_piece1, *tmp_piece2;
	int x, y;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();
	// Using dynamic_cast to perform polymorphic cast
	piece = dynamic_cast<Pawn*>(p);

	// Handles two-square advances
	if (!piece->hasMoved())
		tmp_piece1 = piece->isBlack() ? m_board[x][y - 2]
			: m_board[x][y + 2];
	else
		tmp_piece1 = nullptr;
	// Using +1 and -1 cause depending on the side it moves
	// forward or backward and we want the prior piece
	tmp_piece2 = piece->isBlack() ? m_board[x][y - 1]
		: m_board[x][y + 1];

	if (!tmp_piece2)
	{
		if (piece->isBlack())
			highlight(x, y - 1, MOVE);
		else
			highlight(x, y + 1, MOVE);
		if (!piece->hasMoved() && !tmp_piece1)
			if (piece->isBlack())
				highlight(x, y - 2, MOVE);
			else
				highlight(x, y + 2, MOVE);
	}

	if (piece->isBlack())
	{
		// Prevents trying to access values outside the 8x8 array
		tmp_piece1 = x > 0 && y > 0 ? m_board[x - 1][y - 1] : nullptr;
		tmp_piece2 = x < 7 && y > 0 ? m_board[x + 1][y - 1] : nullptr;
	} else
	{
		// Prevents trying to access values outside the 8x8 array
		tmp_piece1 = x > 0 && y < 7 ? m_board[x - 1][y + 1] : nullptr;
		tmp_piece2 = x < 7 && y < 7 ? m_board[x + 1][y + 1] : nullptr;
	}

	if (tmp_piece1 && piece->isOpponent(tmp_piece1))
		highlight(tmp_piece1->getX(), tmp_piece1->getY(), CAPTURE);
	if (tmp_piece2 && piece->isOpponent(tmp_piece2))
		highlight(tmp_piece2->getX(), tmp_piece2->getY(), CAPTURE);

	// En passant
	if (y == 4 && piece->isWhite() || y == 3 && piece->isBlack())
	{
		if (m_last_move->getPieceType() == PAWN)
		{
			if (abs(m_last_move->fromY() - m_last_move->toY()) == 2
					&& abs(x - m_last_move->fromX()) == 1)
			{
				int x_capt, y_capt;

				x_capt = m_last_move->fromX();
				y_capt = piece->isBlack() ? m_last_move->toY() - 1
					: m_last_move->toY() + 1;
				highlight(x_capt, y_capt, CAPTURE);
			}
		}
	}
}

/**
 * highlightKnightRoutes - Renders the appropriate highlight for the selected
 * knight piece
 *
 * @p: Pointer to the selected knight piece
 * @pinned: Boolean parameter indicating whether or not the piece is pinned
 *
 * Return: Nothing
 */
void ChessBoard::highlightKnightRoutes(Piece* p, bool pinned)
{
	int x, y;
	bool valid;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();
	// Highlights Top-Right square
	if (x <= 6 && y <= 5)
	{
		Piece* piece;

		piece = m_board[x + 1][y + 2];
		valid = !pinned || (pinned && !moveCatastrophy(p, x + 1, y + 2));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x + 1, y + 2, CAPTURE);
			} else
				highlight(x + 1, y + 2, MOVE);
		}
	}

	// Highlights Top-Left square
	if (x >= 1 && y <= 5)
	{
		Piece* piece;

		piece = m_board[x - 1][y + 2];
		valid = !pinned || (pinned && !moveCatastrophy(p, x - 1, y + 2));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x - 1, y + 2, CAPTURE);
			} else
				highlight(x - 1, y + 2, MOVE);
		}
	}

	// Highlights Bottom-Right square
	if (x <= 6 && y >= 2)
	{
		Piece* piece;

		piece = m_board[x + 1][y - 2];
		valid = !pinned || (pinned && !moveCatastrophy(p, x + 1, y - 2));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x + 1, y - 2, CAPTURE);
			} else
				highlight(x + 1, y - 2, MOVE);
		}
	}

	// Highlights Bottom-Left square
	if (x >= 1 && y >= 2)
	{
		Piece* piece;

		piece = m_board[x - 1][y - 2];
		valid = !pinned || (pinned && !moveCatastrophy(p, x - 1, y - 2));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x - 1, y - 2, CAPTURE);
			} else
				highlight(x - 1, y - 2, MOVE);
		}
	}

	// Highlights Right-Top square
	if (x <= 5 && y <= 6)
	{
		Piece* piece;

		piece = m_board[x + 2][y + 1];
		valid = !pinned || (pinned && !moveCatastrophy(p, x + 2, y + 1));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x + 2, y + 1, CAPTURE);
			} else
				highlight(x + 2, y + 1, MOVE);
		}
	}

	// Highlights Right-Bottom square
	if (x <= 5 && y >= 1)
	{
		Piece* piece;

		piece = m_board[x + 2][y - 1];
		valid = !pinned || (pinned && !moveCatastrophy(p, x + 2, y - 1));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x + 2, y - 1, CAPTURE);
			} else
				highlight(x + 2, y - 1, MOVE);
		}
	}

	// Highlights Left-Top square
	if (x >= 2 && y <= 6)
	{
		Piece* piece;

		piece = m_board[x - 2][y + 1];
		valid = !pinned || (pinned && !moveCatastrophy(p, x - 2, y + 1));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x - 2, y + 1, CAPTURE);
			} else
				highlight(x - 2, y + 1, MOVE);
		}
	}

	// Highlights Left-Bottom square
	if (x >= 2 && y >= 1)
	{
		Piece* piece;

		piece = m_board[x - 2][y - 1];
		valid = !pinned || (pinned && !moveCatastrophy(p, x - 2, y - 1));

		if (valid)
		{
			if (piece)
			{
				if (p->isOpponent(piece))
					highlight(x - 2, y - 1, CAPTURE);
			} else
				highlight(x - 2, y - 1, MOVE);
		}
	}
}

/**
 * highlightStraight - Traverses straight in the four directions (up, down,
 * left, and right) highlighting possible squares for the selected piece to
 * move, up until the end of the board or till an obstructing piece is
 * encountererd
 *
 * @p: Pointer to the selected piece
 * @pinned: Boolean parameter indicating whether or not the piece is pinned
 *
 * Return: Nothing
 */
void ChessBoard::highlightStraight(Piece *p, bool pinned)
{
	int x, y;
	bool upper_blk, lower_blk, right_blk, left_blk, valid;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();
	upper_blk = lower_blk = right_blk = left_blk = false;
	for (int x_adv = 1, y_adv = 1; x_adv <= 7 && y_adv <= 7; x_adv++, y_adv++)
	{
		Piece* tmp;

		tmp = nullptr;
		if (!upper_blk && (y + y_adv) <= 7)
		{
			valid = !pinned || (pinned && !moveCatastrophy(p, x, y + y_adv));
			tmp = m_board[x][y + y_adv];
			if (valid)
			{
				if (!tmp)
					highlight(x, y + y_adv, MOVE);
				else
				{
					if (p->isOpponent(tmp))
						highlight(x, y + y_adv, CAPTURE);
					upper_blk = true;
				}
			}
		}

		if (!lower_blk && (y - y_adv) >= 0)
		{
			valid = !pinned || (pinned && !moveCatastrophy(p, x, y - y_adv));
			tmp = m_board[x][y - y_adv];
			if (valid)
			{
				if (!tmp)
					highlight(x, y - y_adv, MOVE);
				else
				{
					if (p->isOpponent(tmp))
						highlight(x, y - y_adv, CAPTURE);
					lower_blk = true;
				}
			}
		}

		if (!right_blk && (x + x_adv) <= 7)
		{
			valid = !pinned || (pinned && !moveCatastrophy(p, x + x_adv, y));
			tmp = m_board[x + x_adv][y];
			if (valid)
			{
				if (!tmp)
					highlight(x + x_adv, y, MOVE);
				else
				{
					if (p->isOpponent(tmp))
						highlight(x + x_adv, y, CAPTURE);
					right_blk = true;
				}
			}
		}

		if (!left_blk && (x - x_adv) >= 0)
		{
			valid = !pinned || (pinned && !moveCatastrophy(p, x - x_adv, y));
			tmp = m_board[x - x_adv][y];
			if (valid)
			{
				if (!tmp)
					highlight(x - x_adv, y, MOVE);
				else
				{
					if (p->isOpponent(tmp))
						highlight(x - x_adv, y, CAPTURE);
					left_blk = true;
				}
			}
		}
	}
}

/**
 * highlightDiagonal - Traverses diagonally in four directions (upper-right,
 * upper-left, lower-right, and lower-left) highlighting possible squares
 * for the selected piece to move, up until the end of the board or till an
 * obstructing piece is encountererd
 *
 * @p: Pointer to the selected piece
 * @pinned: Boolean parameter indicating whether or not the piece is pinned
 *
 * Return: Nothing
 */
void ChessBoard::highlightDiagonal(Piece* p, bool pinned)
{
	int x, y;
	bool ul_blocked, ur_blocked, ll_blocked, lr_blocked, valid;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();
	ul_blocked = ur_blocked = ll_blocked = lr_blocked = false;
	for (int x_adv = 1, y_adv = 1; x_adv <= 7 && y_adv <= 7; x_adv++, y_adv++)
	{
		Piece* tmp;

		tmp = nullptr;
		if (x + x_adv <= 7)
		{
			if (!ur_blocked && (y + y_adv) <= 7)
			{
				valid = !pinned || (pinned && !moveCatastrophy(p, x + x_adv, y + y_adv));
				tmp = m_board[x + x_adv][y + y_adv];
				if (valid)
				{
					if (!tmp)
							highlight(x + x_adv, y + y_adv, MOVE);
					else
					{
						if (p->isOpponent(tmp))
								highlight(x + x_adv, y + y_adv, CAPTURE);
						ur_blocked = true;
					}
				}
			}

			if (!lr_blocked && (y - y_adv) >= 0)
			{
				valid = !pinned || (pinned && !moveCatastrophy(p, x + x_adv, y - y_adv));
				tmp = m_board[x + x_adv][y - y_adv];
				if (valid)
				{
					if (!tmp)
							highlight(x + x_adv, y - y_adv, MOVE);
					else
					{
						if (p->isOpponent(tmp))
								highlight(x + x_adv, y - y_adv, CAPTURE);
						lr_blocked = true;
					}
				}
			}
		}

		if (x - x_adv >= 0)
		{
			if (!ul_blocked && (y + y_adv) <= 7)
			{
				valid = !pinned || (pinned && !moveCatastrophy(p, x - x_adv, y + y_adv));
				tmp = m_board[x - x_adv][y + y_adv];
				if (valid)
				{
					if (!tmp)
							highlight(x - x_adv, y + y_adv, MOVE);
					else
					{
						if (p->isOpponent(tmp))
								highlight(x - x_adv, y + y_adv, CAPTURE);
						ul_blocked = true;
					}
				}
			}

			if (!ll_blocked && (y - y_adv) >= 0)
			{
				valid = !pinned || (pinned && !moveCatastrophy(p, x - x_adv, y - y_adv));
				tmp = m_board[x - x_adv][y - y_adv];
				if (valid)
				{
					if (!tmp)
							highlight(x - x_adv, y - y_adv, MOVE);
					else
					{
						if (p->isOpponent(tmp))
								highlight(x - x_adv, y - y_adv, CAPTURE);
						ll_blocked = true;
					}
				}
			}
		}
	}
}
