#include "../headers/game.h"
#include "../headers/pieces.h"
#include "../headers/moves.h"

/**
 * highlightSquare - Highlights a selected square on the board
 *
 * Return- Nothing
 */
void ChessBoard::highlightSquare(int x, int y)
{
	SDL_Texture* h_texture;
	SDL_Rect h_square;
	float xpos, ypos;
	int p_offset;

	p_offset = 5;
	xpos = x * m_grid_size + m_board_pad + p_offset;
	ypos = y * m_grid_size + m_board_pad + p_offset;
	if ((x + y) % 2 == 0)
		h_texture = loadImage("assets/active_black_piece.png", m_renderer);
	else
		h_texture = loadImage("assets/active_white_piece.png", m_renderer);
	if (h_texture == nullptr)
	{
		SDL_Log("Failed to load active_piece image!!!");
		// Throw Exception
		return;
	}

	h_square = { (int) xpos, (int) ypos, (m_grid_size - p_offset),
		(m_grid_size - p_offset)};
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	SDL_RenderCopy(m_renderer, h_texture, nullptr, &h_square);
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}

void ChessBoard::highlightRoute(Piece* p)
{
	int x, y;

	x = p->getX();
	y = p->getY();
	switch(p->getPieceType())
	{
		case PAWN:
			Pawn* piece;
			Piece* tmp_piece1, *tmp_piece2;

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

			tmp_piece1 = piece->isBlack() ? m_board[x - 1][y - 1]
				: m_board[x - 1][y + 1];
			tmp_piece2 = piece->isBlack() ? m_board[x + 1][y - 1]
				: m_board[x + 1][y + 1];

			// Highlight capture squares
			if (tmp_piece1 && p->isOpponent(tmp_piece1))
				highlightCapture(tmp_piece1->getX(),
						tmp_piece1->getY());
			if (tmp_piece2 && p->isOpponent(tmp_piece2))
				highlightCapture(tmp_piece2->getX(),
						tmp_piece2->getY());

			// En passant
			if (y == 4 && piece->isWhite() || y == 3 && piece->isBlack())
			{
				// Inspect m_last_move values causing segfault
				if (m_last_move->getPieceType() == PAWN)
				{
					if (abs(m_last_move->fromY() - m_last_move->toX()))
					{
						int x_capt, y_capt;

						x_capt = m_last_move->fromX();
						y_capt = piece->isBlack() ? m_last_move->toY() - 1
							: m_last_move->toY() + 1;
						highlightCapture(x_capt, y_capt);
					}
				}
			}
			break;
		case KNIGHT:
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
				}
				else
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
				}
				else
					highlightGrid(x - 2, y - 1);
			}
			break;
		case BISHOP:
			break;
		case ROOK:
			break;
		case QUEEN:
			break;
		case KING:
			break;
		default:
			break;
	}
}

void ChessBoard::highlightGrid(int x, int y)
{
	SDL_Texture* h_texture;
	SDL_Rect h_square;
	float xpos, ypos;
	int p_offset;

	p_offset = 5;
	xpos = x * m_grid_size + m_board_pad + p_offset;
	ypos = y * m_grid_size + m_board_pad + p_offset;
	h_texture = loadImage("assets/possible_squares.png", m_renderer);
	if (h_texture == nullptr)
	{
		SDL_Log("Failed to load active_piece image!!!");
		// Throw Exception
		return;
	}

	h_square = { (int) xpos, (int) ypos, (m_grid_size - p_offset),
		(m_grid_size - p_offset)};
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	SDL_RenderCopy(m_renderer, h_texture, nullptr, &h_square);
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}

void ChessBoard::highlightCapture(int x, int y)
{
	SDL_Texture* h_texture;
	SDL_Rect h_square;
	float xpos, ypos;
	int p_offset;

	p_offset = 5;
	xpos = x * m_grid_size + m_board_pad + p_offset;
	ypos = y * m_grid_size + m_board_pad + p_offset;
	h_texture = loadImage("assets/possible_capture.png", m_renderer);
	if (h_texture == nullptr)
	{
		SDL_Log("Failed to load active_piece image!!!");
		// Throw Exception
		return;
	}

	h_square = { (int) xpos, (int) ypos, (m_grid_size - p_offset),
		(m_grid_size - p_offset)};
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	SDL_RenderCopy(m_renderer, h_texture, nullptr, &h_square);
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}
