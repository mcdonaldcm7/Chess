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
	m_last_move = new Move();
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
	
	m_white_pieces.push_back(m_board[0][0]);
	m_white_pieces.push_back(m_board[7][0]);
	m_white_pieces.push_back(m_board[1][0]);
	m_white_pieces.push_back(m_board[6][0]);
	m_white_pieces.push_back(m_board[2][0]);
	m_white_pieces.push_back(m_board[5][0]);
	m_white_pieces.push_back(m_board[3][0]);
	m_white_pieces.push_back(m_board[4][0]);

	// Initialization of the black pieces in the board
	m_board[0][7] = new Rook(0, 7, true, m_renderer, this);
	m_board[7][7] = new Rook(7, 7, true, m_renderer, this);

	m_board[1][7] = new Knight(1, 7, true, m_renderer, this);
	m_board[6][7] = new Knight(6, 7, true, m_renderer, this);

	m_board[2][7] = new Bishop(2, 7, true, m_renderer, this);
	m_board[5][7] = new Bishop(5, 7, true, m_renderer, this);

	m_board[3][7] = new Queen(3, 7, true, m_renderer, this);
	m_board[4][7] = new King(4, 7, true, m_renderer, this);

	m_black_pieces.push_back(m_board[0][7]);
	m_black_pieces.push_back(m_board[7][7]);
	m_black_pieces.push_back(m_board[1][7]);
	m_black_pieces.push_back(m_board[6][7]);
	m_black_pieces.push_back(m_board[2][7]);
	m_black_pieces.push_back(m_board[5][7]);
	m_black_pieces.push_back(m_board[3][7]);
	m_black_pieces.push_back(m_board[4][7]);

	// Loop to add pawns to the board
	for (int i = 0; i < 8; i++)
	{
		m_board[i][1] = new Pawn(i, 1, false, m_renderer, this);
		m_white_pieces.push_back(m_board[i][1]);
		m_board[i][6] = new Pawn(i, 6, true, m_renderer, this);
		m_black_pieces.push_back(m_board[i][6]);
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
			xpos = floor(x * m_grid_size + m_board_pad) + p_offset;
			ypos = floor(y * m_grid_size + m_board_pad) + p_offset;
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

/**
 * setLastMove - Sets m_last_move to the last move played
 *
 * @from_x: intial x position of piece before move
 * @to_x: x position of piece after move
 * @from_y: initial y position of piece before move
 * @to_y: y position of piece after move
 * @type: Type of piece that made the last move e.g PAWN
 */
void ChessBoard::setLastMove(int from_x, int to_x, int from_y, int to_y, PieceType type)
{
	m_last_move->setFromX(from_x);
	m_last_move->setToX(to_x);
	m_last_move->setFromY(from_y);
	m_last_move->setToY(to_y);
	m_last_move->setPieceType(type);
}

ChessBoard::~ChessBoard()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (m_board[x][y] != nullptr)
			{
				delete (m_board[x][y]);
				m_board[x][y] = nullptr;
			}
		}
	}
	delete m_last_move;
	SDL_DestroyTexture(m_chess_board);
}
