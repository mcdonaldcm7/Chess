#ifndef PIECES_H_
#define PIECES_H_

#include "view.h"
#include <iostream>

enum PieceType
{
	KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE
};

class ChessBoard;
class Grid;

/*
 * class Piece - The base class for all chess pieces
 *
 * @isBlack: Is piece black or not
 * @x: x position of piece
 * @y: y position of piece
 * @pieceTexture: Texture representation of piece to be loaded from image
 *
 * Description: Base class declaration for all pieces
 */
class Piece {
	protected:
		bool is_black;
		int x;
		int y;
		PieceType piece_type;
		SDL_Texture* piece_texture;
		ChessBoard* m_board;

	public:
		Piece(int, int, bool, SDL_Renderer*, ChessBoard*);
		~Piece();
		virtual bool canMove(int, int, Piece* p = nullptr) = 0;
		int getX() const { return (x); };
		int getY() const { return (y); };
		void setX(int);
		void setY(int);
		// Debugging purposes
		void printBoard(void);
		bool isBlack(void) const { return (is_black); };
		bool isWhite(void) const { return (!is_black); };
		bool isOpponent(const Piece*) const;
		bool isCovered(void);
		bool canAttack(Piece*);
		Grid* interceptGrid(Piece*, Piece*);
		bool operator==(const Piece*) const;
		PieceType getPieceType(void) const { return (piece_type); };
		SDL_Texture* getTexture(void) const { return (piece_texture); };
};

class King : public Piece {
	private:
		bool m_has_moved;
		bool m_on_check;
		Piece* m_attacker;
	public:
		King(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
		void setAttacker(Piece* piece) { m_attacker = piece; };
		Piece* getAttacker(void) const { return (m_attacker); };
		void moved(void) { m_has_moved = true; };
		void setCheck(bool on_check) { m_on_check = on_check; };
		bool onCheck(void) const { return (m_on_check); };
		bool hasMoved(void) const { return (m_has_moved); };
};

class Queen : public Piece {
	private:
		int angle(int, int);
	public:
		Queen(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
};

class Rook : public Piece {
	private:
		bool m_has_moved;
	public:
		Rook(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
		void moved(void) { m_has_moved = true; };
		bool hasMoved(void) const { return (m_has_moved); };
};

class Bishop : public Piece {
	private:
		int angle(int, int);
	public:
		Bishop(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
};

class Knight : public Piece {
	public:
		Knight(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
};

class Pawn : public Piece {
	public:
		Pawn(int, int, bool, SDL_Renderer*, ChessBoard*);
		bool canMove(int, int, Piece*) override;
		bool hasMoved(void) const;
};

// Overload the << operator to print Piece objects
std::ostream& operator<<(std::ostream&, const Piece&);
#endif
