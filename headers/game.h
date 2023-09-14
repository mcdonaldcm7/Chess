#ifndef GAME_H_
#define GAME_H_

class Piece;

#include "view.h"
#include "moves.h"
#include <stdio.h>
#include <vector>

enum HighlightType { PIECE, MOVE, CAPTURE, CASTLING };

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
		int m_board_size;
		int m_grid_size;
		float m_board_pad;
		bool m_black_turn;
		std::vector<Piece*> m_black_pieces;
		std::vector<Piece*> m_white_pieces;
		Piece* m_board[8][8];
		Move* m_last_move;
		SDL_Texture* m_chess_board;
		SDL_Renderer* m_renderer;

	public:
		King* m_black_king;
		King* m_white_king;

		ChessBoard(SDL_Renderer*, const int);
		~ChessBoard(void);
		void initBoard(void);
		void drawBoard(void);

		void highlight(int, int, HighlightType);
		void highlightRoute(Piece*);

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
		bool underCheck(Piece*);
		bool blackTurn(void) { return (m_black_turn); };
		void check(void);
		bool isPieceTurn(const Piece*);

		void flipTurn(void) { m_black_turn = !m_black_turn; };

		std::vector<Piece*> getBlackPieces(void) const { return (m_black_pieces); };
		std::vector<Piece*> getWhitePieces(void) const { return (m_white_pieces); };

		SDL_Texture* getChessBoard(void) const { return (m_chess_board); };
		Piece* getPiece(int x, int y) const { return (m_board[x][y]); };
		King* pieceKing(Piece*);

		Piece* trackDiagonal(Piece*, int, int);
		Piece* trackStraight(Piece*, int, int);
		Piece* routeBlocked(Piece*, int, int);

		Move* getLastMove(void) const { return (m_last_move); };
};

void start(void);
void eventHandler(SDL_Event*);
bool isPieceTurn(const Piece*);

#endif
