#include "../headers/pieces.h"
#include "../headers/game.h"

Rook::Rook(int x, int y, bool isBlack, SDL_Renderer* renderer, ChessBoard* board)
	: Piece(x, y, isBlack, renderer, board)
{
	piece_type = PieceType::ROOK;
	if (isBlack)
	{
		piece_texture = loadImage("assets/black_rook.png",
				renderer);
	} else
	{
		piece_texture = loadImage("assets/white_rook.png",
				renderer);
	}
	m_has_moved = false;
}

bool Rook::canMove(int x_dest, int y_dest)
{
	Piece* tmp;

	tmp = m_board->getPiece(x_dest, y_dest);
	if (abs(x - x_dest) == 0 && abs(y - y_dest) > 0 ||
			abs(x - x_dest) > 0 && abs(y - y_dest) == 0)
	{
		// Moving Vertically
		// Consider case where piece is defending the king
		if ((!tmp || isOpponent(tmp)))
		{
			Piece* blocker;

			blocker = m_board->routeBlocked(this, x_dest, y_dest);
			if (blocker)
			{
				/*switch (blocker->getPieceType())
				{
					case ROOK:
						SDL_Log("Blocked by a rook on gird x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
					case PAWN:
						SDL_Log("Blocked by a pawn on grid x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
					case KNIGHT:
						SDL_Log("Blocked by a knight on grid x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
					case BISHOP:
						SDL_Log("Blocked by a bishop on grid x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
					case QUEEN:
						SDL_Log("Blocked by a queen on grid x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
					case KING:
						SDL_Log("Blocked by a king on grid x %d, y %d\n",
								blocker->getX(), blocker->getY());
						break;
				}*/
				return (false);
			}
			return (true);
		}
	}
	return (false);
}
