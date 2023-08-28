#include "../headers/game.h"
#include "../headers/pieces.h"
#include <cmath>

// Used to check how many times drawBoard was called per frame
//static int counter = 0;

ChessBoard::ChessBoard(SDL_Renderer* renderer, const int board_size)
{
	m_renderer = renderer;
	if (!m_renderer)
	{
		printf("Invalid Renderer provided!");
		// Throw Exception
	}
	m_chess_board = loadImage("assets/chess-board.png", m_renderer);
	if (m_chess_board == nullptr)
	{
		printf("Failed to load Image!!!\n");
		// Throw Exception
	}
	m_board_size = board_size;
	// Border padding constitues roughly 3.125% (each side) of board image,
	// Use simple calculation to remove it
	m_board_pad = ceil (board_size * 3.125) / 100;

	// Obtain the size of each grid
	m_grid_size = floor((board_size - m_board_pad * 2) / 8);
	initBoard();
}

/**
 * init_board - Initializes m_board with the pieces
 *
 * Return: Void function returns nothing
 */
void ChessBoard::initBoard(void)
{
	// Initialize chess board to avoid accessing garbage values
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			m_board[x][y] = nullptr;

	// Initialization of the white pieces in the board
	m_board[0][0] = new Rook(0, 0, false, m_renderer, this);
	m_board[7][0] = new Rook(7, 0, false, m_renderer, this);

	m_board[1][0] = new Knight(1, 0, false, m_renderer, this);
	m_board[6][0] = new Knight(6, 0, false, m_renderer, this);

	m_board[2][0] = new Bishop(2, 0, false, m_renderer, this);
	m_board[5][0] = new Bishop(5, 0, false, m_renderer, this);

	m_board[3][0] = new Queen(3, 0, false, m_renderer, this);
	m_board[4][0] = new King(4, 0, false, m_renderer, this);

	// Initialization of the black pieces in the board
	m_board[0][7] = new Rook(0, 7, true, m_renderer, this);
	m_board[7][7] = new Rook(7, 7, true, m_renderer, this);

	m_board[1][7] = new Knight(1, 7, true, m_renderer, this);
	m_board[6][7] = new Knight(6, 7, true, m_renderer, this);

	m_board[2][7] = new Bishop(2, 7, true, m_renderer, this);
	m_board[5][7] = new Bishop(5, 7, true, m_renderer, this);

	m_board[3][7] = new Queen(3, 7, true, m_renderer, this);
	m_board[4][7] = new King(4, 7, true, m_renderer, this);

	// Loop to add pawns to the board
	for (int i = 0; i < 8; i++)
	{
		m_board[i][1] = new Pawn(i, 1, false, m_renderer, this);
		m_board[i][6] = new Pawn(i, 6, true, m_renderer, this);
	}
}

/**
 * drawBoard - Draws every piece in m_board on the board
 *
 * Return: Nothing
 */
void ChessBoard::drawBoard(void)
{
	// Offset used to adjust piece size and position in grid
	int p_offset;

	p_offset = 5;

	// Clear window renderer
	SDL_RenderClear(m_renderer);
	// Define the position and size the board should be drawn using a rectangle
	const SDL_Rect fillRect = { 0, 0, m_board_size,
		m_board_size };
	// Draws chess board onto the window so pieces can be drawn onto the board
	SDL_RenderCopy(m_renderer, m_chess_board,
			nullptr, &fillRect);
	// SDL_RenderPresent(m_renderer); Uncomment this and board starts to flicker

	// Set the render target to the chess board
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			SDL_Rect piece_square;
			float xpos, ypos;

			if (m_board[x][y] == nullptr)
				continue;
			xpos = x * m_grid_size + m_board_pad + p_offset;
			ypos = y * m_grid_size + m_board_pad + p_offset;
			piece_square = {(int) xpos, (int) ypos,
				m_grid_size - p_offset,
				m_grid_size - p_offset };
			SDL_RenderCopy(m_renderer, m_board[x][y]->getTexture(),
					NULL, &piece_square);
		}
	}
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}

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

	h_square = {xpos, ypos, (m_grid_size - p_offset),
		(m_grid_size - p_offset)};
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	SDL_RenderCopy(m_renderer, h_texture, nullptr, &h_square);
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}

ChessBoard::~ChessBoard()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (m_board[x][y] == nullptr)
				continue;
			delete (m_board[x][y]);
		}
	}
	SDL_DestroyTexture(m_chess_board);
}
