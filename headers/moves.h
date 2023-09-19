#ifndef MOVES_H_
#define MOVES_H_

enum PieceType
{
	KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE
};

/**
 * Move - Defines a move made by a player
 *
 * @from_x: initial x position of piece
 * @to_x: x position to move piece to
 * @from_y: initial y position of piece
 * @to_y: y position of piece to move to
 * @piece_type: Piece type
 */
class Move {
private:
	int from_x, to_x, from_y, to_y;
	PieceType piece_type;

public:
	Move();

	int fromX(void) const
	{
		return (from_x);
	};

	int fromY(void) const
	{
		return (from_y);
	};

	int toX(void) const
	{
		return (to_x);
	};

	int toY(void) const
	{
		return (to_y);
	};

	PieceType getPieceType(void) const
	{
		return (piece_type);
	};

	void setFromX(int);
	void setFromY(int);
	void setToX(int);
	void setToY(int);

	void setPieceType(PieceType pt)
	{
		this->piece_type = pt;
	};
};

/**
 * Grid - Defines the position of a grid on the chess board
 *
 * @x: x-axis position of the grid on the chess board
 * @y: y-axis position of the grid on the chess board
 */
class Grid {
private:
	int x, y;

public:
	Grid() : x(-1), y(-1) {};
	Grid(int x_pos, int y_pos) : x(x_pos), y(y_pos) {};
	int getX(void) const
	{
		return (x);
	};

	int getY(void) const
	{
		return (y);
	};

	void setX(int x)
	{
		this->x = x;
	};

	void setY(int y)
	{
		this->y = y;
	};
};
#endif
