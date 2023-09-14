#include "../headers/pieces.h"
#include "../headers/game.h"

/**
 * Piece - Initialized member variables of all Piece subclasses
 *
 * @x: Initial x position of the object
 * @y: Initial y position of the object
 * @isBlack: Is piece black or white
 * @renderer: Renderer for object to use to render itself
 * @board: Chess board the piece belongs to
 *
 * Return: Nothing
 */
Piece::Piece(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
{
	this->x = x;
	this->y = y;
	this->is_black = isBlack;
	m_board = board;
}

/**
 * setX - Sets the x position of the piece
 *
 * @x: x-axis position to set piece's x to
 *
 * Return: Nothing
 */
void Piece::setX(int x)
{
	this->x = x;
}

/**
 * setY - Sets the y position of the piece
 *
 * @y: y-axis position to set piece's y to
 *
 * Return: Nothing
 */
void Piece::setY(int y)
{
	this->y = y;
}

/**
 * operator== - Overloading the equal to operator to be able to compare two
 * Piece objects
 *
 * @piece: Other piece to compare self to
 *
 * Return: true if this == piece, false Otherwise
 */
bool Piece::operator==(const Piece* piece) const
{
	return (x == piece->getX() &&
			y == piece->getY() &&
			piece_type == piece->getPieceType());
}

Piece::~Piece(void)
{
	SDL_DestroyTexture(piece_texture);
}
