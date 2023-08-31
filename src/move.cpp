#include "../headers/moves.h"

Move::Move()
{
	this->from_x = -1;
	this->to_x = -1;
	this->from_y = -1;
	this->to_y = -1;
	this->piece_type = NONE;
}

/**
 * setFromX - sets the from_x member of the calling object to the from_x
 * argument
 *
 * @from_x: Value of from_x
 *
 * Return: Nothing
 */
void Move::setFromX(int from_x)
{
	this->from_x = from_x;
}

/**
 * setToX - sets the to_x member of the calling object to the to_x argument
 *
 * @to_x: Value of to_x
 *
 * Return: Nothing
 */
void Move::setToX(int to_x)
{
	this->to_x = to_x;
}

/**
 * setFromY - sets the from_y member of the calling object to the from_y
 * argument
 *
 * @from_y: Value of from_y
 *
 * Return: Nothing
 */
void Move::setFromY(int from_y)
{
	this->from_y = from_y;
}

/**
 * setToY - sets the to_y member of the calling object to the to_y argument
 *
 * @to_y: Value of to_y
 *
 * Return: Nothing
 */
void Move::setToY(int to_y)
{
	this->to_y = to_y;
}
