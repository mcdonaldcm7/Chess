#include "../headers/game.h"
#include "../headers/pieces.h"
#include "../headers/moves.h"

/**
 * highlightSquare - Highlights a selected piece on the board
 *
 * @x: x position of piece to highlight
 * @y: y position of piece to highlight
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

/**
 * highlightGrid - Highlights possible squares for a piece to move to on the
 * board
 *
 * @x: x position of grid on the board
 * @y: y position of grid on the board
 *
 * Return: Nothing
 */
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

/**
 * highlightCapture - Highlights possible captures for a piece on the board
 *
 * @x: x position of grid on the board
 * @y: y position of grid on the board
 *
 * Return: Nothing
 */
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

/**
 * highlightRoute - Highlights the route of different squares a piece can move
 * to on the board
 *
 * @p: Piece whoose routes should be highlighted
 *
 * Return: Nothing
 */
void ChessBoard::highlightRoute(Piece* p)
{
	int x, y;

	x = p->getX();
	y = p->getY();
	switch(p->getPieceType())
	{
		case PAWN:
			highlightPawnRoutes(p);
			break;
		case KNIGHT:
			highlightKnightRoutes(p);
			break;
		case BISHOP:
			highlightDiagonal(p);
			break;
		case ROOK:
			highlightStraight(p);
			break;
		case QUEEN:
			highlightDiagonal(p);
			highlightStraight(p);
			break;
		case KING:
			break;
		default:
			break;
	}
}