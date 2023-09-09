#include "../headers/game.h"

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
			highlightGrid(x, y - 1);
		else
			highlightGrid(x, y + 1);
		if (!piece->hasMoved() && !tmp_piece1)
			if (piece->isBlack())
				highlightGrid(x, y - 2);
			else
				highlightGrid(x, y + 2);
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
		highlightCapture(tmp_piece1->getX(), tmp_piece1->getY());
	if (tmp_piece2 && piece->isOpponent(tmp_piece2))
		highlightCapture(tmp_piece2->getX(), tmp_piece2->getY());

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
				highlightCapture(x_capt, y_capt);
			}
		}
	}
}

/**
 * highlightKnightRoutes - Renders the appropriate highlight for the selected
 * knight piece
 *
 * @p: Pointer to the selected knight piece
 *
 * Return: Nothing
 */
void ChessBoard::highlightKnightRoutes(Piece* p)
{
	int x, y;

	x = p->getX();
	y = p->getY();
	// Highlights Top-Right square
	if (x <= 6 && y <= 5)
	{
		Piece* piece;

		piece = m_board[x + 1][y + 2];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x + 1, y + 2);
		} else
			highlightGrid(x + 1, y + 2);
	}

	// Highlights Top-Left square
	if (x >= 1 && y <= 5)
	{
		Piece* piece;

		piece = m_board[x - 1][y + 2];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x - 1, y + 2);
		} else
			highlightGrid(x - 1, y + 2);
	}

	// Highlights Bottom-Right square
	if (x <= 6 && y >= 2)
	{
		Piece* piece;

		piece = m_board[x + 1][y - 2];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x + 1, y - 2);
		} else
			highlightGrid(x + 1, y - 2);
	}

	// Highlights Bottom-Left square
	if (x >= 1 && y >= 2)
	{
		Piece* piece;

		piece = m_board[x - 1][y - 2];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x - 1, y - 2);
		} else
			highlightGrid(x - 1, y - 2);
	}

	// Highlights Right-Top square
	if (x <= 5 && y <= 6)
	{
		Piece* piece;

		piece = m_board[x + 2][y + 1];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x + 2, y + 1);
		} else
			highlightGrid(x + 2, y + 1);
	}

	// Highlights Right-Bottom square
	if (x <= 5 && y >= 1)
	{
		Piece* piece;

		piece = m_board[x + 2][y - 1];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x + 2, y - 1);
		} else
			highlightGrid(x + 2, y - 1);
	}

	// Highlights Left-Top square
	if (x >= 2 && y <= 6)
	{
		Piece* piece;

		piece = m_board[x - 2][y + 1];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x - 2, y + 1);
		} else
			highlightGrid(x - 2, y + 1);
	}

	// Highlights Left-Bottom square
	if (x >= 2 && y >= 1)
	{
		Piece* piece;

		piece = m_board[x - 2][y - 1];
		if (piece)
		{
			if (p->isOpponent(piece))
				highlightCapture(x - 2, y - 1);
		} else
			highlightGrid(x - 2, y - 1);
	}
}

/**
 * highlightStraight - Traverses straight in the four directions (up, down,
 * left, and right) highlighting possible squares for the selected piece to
 * move, up until the end of the board or till an obstructing piece is
 * encountererd
 *
 * @p: Pointer to the selected piece
 *
 * Return: Nothing
 */
void ChessBoard::highlightStraight(Piece *p)
{
	int x, y;
	bool upper_blk, lower_blk, right_blk, left_blk;

	x = p->getX();
	y = p->getY();
	upper_blk = lower_blk = right_blk = left_blk = false;
	for (int x_adv = 1, y_adv = 1; x_adv <= 7 && y_adv <= 7; x_adv++, y_adv++)
	{
		Piece* tmp;

		tmp = nullptr;
		if (!upper_blk && (y + y_adv) <= 7)
		{
			tmp = m_board[x][y + y_adv];
			if (!tmp)
				highlightGrid(x, y + y_adv);
			else
			{
				if (p->isOpponent(tmp))
					highlightCapture(x, y + y_adv);
				upper_blk = true;
			}
		}

		if (!lower_blk && (y - y_adv) >= 0)
		{
			tmp = m_board[x][y - y_adv];
			if (!tmp)
				highlightGrid(x, y - y_adv);
			else
			{
				if (p->isOpponent(tmp))
					highlightCapture(x, y - y_adv);
				lower_blk = true;
			}
		}

		if (!right_blk && (x + x_adv) <= 7)
		{
			tmp = m_board[x + x_adv][y];
			if (!tmp)
				highlightGrid(x + x_adv, y);
			else
			{
				if (p->isOpponent(tmp))
					highlightCapture(x + x_adv, y);
				right_blk = true;
			}
		}

		if (!left_blk && (x - x_adv) >= 0)
		{
			tmp = m_board[x - x_adv][y];
			if (!tmp)
				highlightGrid(x - x_adv, y);
			else
			{
				if (p->isOpponent(tmp))
					highlightCapture(x - x_adv, y);
				left_blk = true;
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
 *
 * Return: Nothing
 */
void ChessBoard::highlightDiagonal(Piece* p)
{
	int x, y;
	bool ul_blocked, ur_blocked, ll_blocked, lr_blocked;

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
				tmp = m_board[x + x_adv][y + y_adv];
				if (!tmp)
					highlightGrid(x + x_adv, y + y_adv);
				else
				{
					if (p->isOpponent(tmp))
						highlightCapture(x + x_adv, y + y_adv);
					ur_blocked = true;
				}
			}

			if (!lr_blocked && (y - y_adv) >= 0)
			{
				tmp = m_board[x + x_adv][y - y_adv];
				if (!tmp)
					highlightGrid(x + x_adv, y - y_adv);
				else
				{
					if (p->isOpponent(tmp))
						highlightCapture(x + x_adv, y - y_adv);
					lr_blocked = true;
				}
			}
		}

		if (x - x_adv >= 0)
		{
			if (!ul_blocked && (y + y_adv) <= 7)
			{
				tmp = m_board[x - x_adv][y + y_adv];
				if (!tmp)
					highlightGrid(x - x_adv, y + y_adv);
				else
				{
					if (p->isOpponent(tmp))
						highlightCapture(x - x_adv, y + y_adv);
					ul_blocked = true;
				}
			}

			if (!ll_blocked && (y - y_adv) >= 0)
			{
				tmp = m_board[x - x_adv][y - y_adv];
				if (!tmp)
					highlightGrid(x - x_adv, y - y_adv);
				else
				{
					if (p->isOpponent(tmp))
						highlightCapture(x - x_adv, y - y_adv);
					ll_blocked = true;
				}
			}
		}
	}
}
