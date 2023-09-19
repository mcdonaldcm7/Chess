#include "../headers/game.h"
#include "../headers/pieces.h"

// Overload the << operator to print Piece objects
std::ostream& operator<<(std::ostream& os, const Piece& piece) {
    os << "Piece: ";
    os << "Type: ";
    switch (piece.getPieceType()) {
        case KING:   os << "King";   break;
        case QUEEN:  os << "Queen";  break;
        case ROOK:   os << "Rook";   break;
        case BISHOP: os << "Bishop"; break;
        case KNIGHT: os << "Knight"; break;
        case PAWN:   os << "Pawn";   break;
        case NONE:   os << "None";   break;
    }
    os << ", Position: (" << piece.getX() << ", " << piece.getY() << ")";
    os << ", Color: " << (piece.isBlack() ? "Black" : "White");
    return os;
}

/**
 * isSafe - Checks if a square is safe for a piece to jump to
 *
 * @piece: Piece scouting the safety of the square
 * @x_scout: x-axis position to scout
 * @y_scout: y-axis position to scout
 *
 * Return: true if the grid (x_scout, y_scout) is safe, false Otherwise
 */
bool ChessBoard::isSafe(Piece *piece, int x_scout, int y_scout)
{
	std::vector<Piece*> foes;

	foes = piece->isBlack() ? getWhitePieces() : getBlackPieces();
	if (m_board[x_scout][y_scout])
		return (false);
	for (Piece* p : foes)
	{
		PieceType p_type;

		p_type = p->getPieceType();

		if (p_type == PAWN)
		{
			// Handles pawn piece peculiar case: Pawns can't capture on all the squares
			// they can move to, So a custom implementation is required

			if (abs(x_scout - p->getX()) == 1 && abs(y_scout - p->getY()) == 1)
				return (false);
		} else if (p_type == KING)
		{
			// Handles king piece, using the king's canMove function will result in an
			// unending loop

			if (abs(x_scout - p->getX()) <= 1 && abs(y_scout - p->getY()) <= 1)
				return (false);
		} else if (p->canMove(x_scout, y_scout, piece))
			return (false);
	}
	return (true);
}

/**
 * check - Updates the check and attacker member variables of the king whoose
 * color's turn it is
 *
 * @ignore: Pieces to ignore when searching for threats
 *
 * Return: Nothing
 */
void ChessBoard::check(Piece *ignore)
{
	int king_x, king_y;

	if (m_black_turn)
	{
		king_x = m_black_king->getX();
		king_y = m_black_king->getY();

		for (Piece* p : m_white_pieces)
		{
			if (p->getPieceType() != KING &&
					p != ignore &&
					p->canMove(king_x, king_y))
			{
				m_black_king->setCheck(true);
				m_black_king->setAttacker(p);
				return;
			}
		}

		m_black_king->setCheck(false);
		m_black_king->setAttacker(nullptr);
	} else
	{
		king_x = m_white_king->getX();
		king_y = m_white_king->getY();

		for (Piece* p : m_black_pieces)
		{
			if (p->getPieceType() != KING &&
					p != ignore &&
					p->canMove(king_x, king_y))
			{
				m_white_king->setCheck(true);
				m_white_king->setAttacker(p);
				return;
			}
		}

		m_white_king->setCheck(false);
		m_white_king->setAttacker(nullptr);
	}
}

/**
 * underCheck - Checks if the king of the specified Piece parameter p is under
 * check
 *
 * @p: Piece to check
 *
 * Return: true is p's king is under check, false otherwise
 */
bool ChessBoard::underCheck(Piece *p)
{
	if (!p)
		return (false);
	return ((p->isBlack() && m_black_king->onCheck()) || (p->isWhite() &&
			m_white_king->onCheck()));
}

/**
 * pieceKing - Return the king of the specified parameter piece
 *
 * @piece: Piece whoose king is to be returned
 *
 * Return: Pointer to king for the specified piece
 */
King* ChessBoard::pieceKing(Piece *piece)
{
	return (piece->isBlack() ? m_black_king : m_white_king);
};

/**
 * isPiecePinned - Checks if a piece is pinned to it's position
 *
 * @piece: Piece to check
 *
 * Return: true if piece is pinned, false otherwise
 */
bool ChessBoard::isPiecePinned(Piece *piece)
{
	int piece_x, piece_y;
	bool pinned;
	King* king;

	if (!piece || piece->getPieceType() == KING)
		return (false);

	piece_x = piece->getX();
	piece_y = piece->getY();

	m_board[piece_x][piece_y] = nullptr;
	check();
	pinned = underCheck(piece);
	m_board[piece_x][piece_y] = piece;
	check();

	return (pinned);
}
