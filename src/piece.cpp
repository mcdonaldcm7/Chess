#include "../headers/pieces.h"
#include "../headers/game.h"

/**
 * Piece - Initialized member variables of all Piece subclasses
 *
 * @renderer: Renderer for object to use to render itself
 * @x: Initial x position of the object
 * @y: Initial y position of the object
 * @isBlack: Is piece black or white
 * @piece_type: Used to set the piece type based on the PieceType enum
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
 * Return: Nothing
 */
void Piece::setX(int x)
{
	this->x = x;
}

/**
 * setY - Sets the y position of the piece
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

/**
 * isOpponent - Checks if a piece belongs to your opponent
 *
 * @p: Pointer to piece to compare color
 *
 * Return: true if piece pointed to the this keyword and p are of the same
 * color, false Otherwise
 */
bool Piece::isOpponent(const Piece* p) const
{
	if (this->isBlack() && p->isBlack() ||
			this->isWhite() && p->isWhite())
		return (false);
	return (true);
}

/**
 * isCovered - Checks if a piece is protected i.e can any of it's allies move to
 * the square it's occupying
 *
 * Return: true if piece is protected, false Otherwise
 */
bool Piece::isCovered(void)
{
	std::vector<Piece*> allies;

	allies = isBlack() ? m_board->getBlackPieces() : m_board->getWhitePieces();
	for (Piece* p : allies)
		if (p->canMove(x, y, this))
			return (true);
	return (false);
}

/**
 * ~Piece - Handles releasing of resources when the object gets destroyed
 *
 * Return: Nothing
 */
Piece::~Piece(void)
{
	SDL_DestroyTexture(piece_texture);
}
