#ifndef GAME_H_
#define GAME_H_

class Piece;

#include "view.h"
#include "moves.h"
#include <stdio.h>

/**
 * ChessBoard - Groups the board and all methods required to run a chess
 * together in a class
 *
 * @m_board: A two-dimensional representation of the chess board
 * @m_renderer: SDL_Renderer to use to render the pieces onto the board
 * @m_board_size: Size of the board
 */
class ChessBoard {
	private:
		Piece* m_board[8][8];
		SDL_Renderer* m_renderer;
		int m_board_size;
		int m_grid_size;
		float m_board_pad;
		Move* m_last_move;
		SDL_Texture* m_chess_board;


	public:
		ChessBoard(SDL_Renderer*, const int);
		~ChessBoard(void);
		void initBoard(void);
		void drawBoard(void);
		void highlightSquare(int, int);
		void highlightRoute(Piece*);
		void highlightGrid(int, int);
		void highlightCapture(int, int);
		void movePiece(Piece*, int, int);
		void setLastMove(int, int, int, int, PieceType);
		int getBoardPad() const { return (m_board_pad); };
		int getGridSize() const { return (m_grid_size); };
		SDL_Texture* getChessBoard(void) const { return (m_chess_board); };
		Piece* getPiece(int x, int y) const { return (m_board[x][y]); };
		Piece* trackDiagonal(Piece*, int, int);
		Piece* trackStraight(Piece*, int, int);
		Piece* routeBlocked(Piece*, int, int);
		Move* getLastMove(void) const { return (m_last_move); };
};

void start(void);
void eventHandler(SDL_Event*);
bool isPieceTurn(const Piece*);

#endif
