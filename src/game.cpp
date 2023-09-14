#include "../headers/game.h"
#include "../headers/pieces.h"
#include <stdio.h>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BOARD_SIZE = 600;
SDL_Window* window;
SDL_Renderer* window_renderer;
ChessBoard* board;
bool quit;
Piece* active_piece;

/**
 * start - Starts the game handling window initialization and making
 * appropriate call to event handlers
 *
 * Return: Nothing
 */
void start(void)
{
	if (init())
	{
		// Create window and assign it window variable
		window = createWindow("Chess", SCREEN_WIDTH, SCREEN_HEIGHT,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			printf("window == nullptr\n");
			return;
		}
		// Create window renderer
		window_renderer = createRenderer(window, SDL_RENDERER_ACCELERATED);
		if (window_renderer == nullptr)
		{
			printf("window_renderer == nullptr\n");
			return;
		}
		board = new ChessBoard(window_renderer, BOARD_SIZE);
		active_piece = nullptr;
		quit = false;

		board->drawBoard();
		while (!quit)
		{
			SDL_Event event;

			if (SDL_WaitEvent(&event))
				eventHandler(&event);
		}
	}
	wrapUp();
}

/**
 * eventHandler - Handles the game events
 *
 * @event: SDL_Event pointer to use to handle the type of event that was
 * invoked
 *
 * Return: Nothing
 */
void eventHandler(SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			int x = event->button.x;
			int y = event->button.y;
			int pad = board->getBoardPad();
			Piece* clicked_piece;

			if ((x > pad) && (x <= (BOARD_SIZE - pad)) &&
					(y > pad) && (y <= (BOARD_SIZE - pad)))
			{
				int grid_x_pos = (int) floor((x - pad) / board->getGridSize());
				int grid_y_pos = (int) floor((y - pad) / board->getGridSize());
				clicked_piece = board->getPiece(grid_x_pos, grid_y_pos);

				if (clicked_piece == nullptr)
				{
					if (active_piece != nullptr && board->isPieceTurn(active_piece))
					{
						if (active_piece->canMove(grid_x_pos, grid_y_pos))
						{
							board->setLastMove(active_piece->getX(), grid_x_pos,
									active_piece->getY(), grid_y_pos,
									active_piece->getPieceType());
							board->movePiece(active_piece, grid_x_pos, grid_y_pos);
							active_piece = nullptr;
							board->flipTurn();
						}
					}
				} else
				{
					if (active_piece == nullptr)
					{
						if (board->isPieceTurn(clicked_piece))
						{
							active_piece = clicked_piece;
							board->highlight(grid_x_pos, grid_y_pos, PIECE);
							board->highlightRoute(active_piece);
						}
					} else
					{
						if (clicked_piece == active_piece)
						{
							active_piece = nullptr;
							board->drawBoard();
						} else
						{
							if (active_piece->canMove(grid_x_pos, grid_y_pos))
							{
								board->setLastMove(active_piece->getX(), grid_x_pos,
										active_piece->getY(), grid_y_pos,
										active_piece->getPieceType());
								board->movePiece(active_piece, grid_x_pos, grid_y_pos);
								active_piece = nullptr;
								board->flipTurn();
							}
						}
					}
				}
			} else
			{
				// Handles clicks outside the boundaries of the board
				// Such as the game menu, timer, etc.
			}
	}
}
