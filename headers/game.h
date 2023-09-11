#ifndef GAME_H_
#define GAME_H_

class Piece;

#include "view.h"
#include "moves.h"
#include <stdio.h>
#include <vector>

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
		std::vector<Piece*> m_black_pieces;
		std::vector<Piece*> m_white_pieces;
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
		void highlightCastling(int, int);
		void movePiece(Piece*, int, int);
		void setLastMove(int, int, int, int, PieceType);
		void highlightKingRoutes(Piece*);
		void highlightPawnRoutes(Piece*);
		void highlightKnightRoutes(Piece*);
		void highlightStraight(Piece*);
		void highlightDiagonal(Piece*);
		int getBoardPad(void) const { return (m_board_pad); };
		int getGridSize(void) const { return (m_grid_size); };
		bool isSafe(Piece*, int, int);
		std::vector<Piece*> getBlackPieces(void) const { return (m_black_pieces); };
		std::vector<Piece*> getWhitePieces(void) const { return (m_white_pieces); };
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
