#include "../headers/game.h"
#include "../headers/pieces.h"
#include "../headers/moves.h"

/**
 * highlight - Handles the highlighting of squares based on the highlight type
 *
 * @x: x position of piece to highlight
 * @y: y position of piece to highlight
 * @type: Indicates the type of highlight to use
 *
 * Return- Nothing
 */
void ChessBoard::highlight(int x, int y, HighlightType type)
{
	SDL_Texture* h_texture;
	SDL_Rect h_square;
	float xpos, ypos;
	int p_offset;

	p_offset = 5;
	xpos = x * m_grid_size + m_board_pad + p_offset;
	ypos = y * m_grid_size + m_board_pad + p_offset;


	if (type == PIECE)
		if ((x + y) % 2 == 0)
			h_texture = loadImage("assets/active_black_piece.png", m_renderer);
		else
			h_texture = loadImage("assets/active_white_piece.png", m_renderer);
	else if (type == MOVE)
		h_texture = loadImage("assets/possible_squares.png", m_renderer);
	else if (type == CAPTURE)
		h_texture = loadImage("assets/possible_capture.png", m_renderer);
	else if (type == CASTLING)
		h_texture = loadImage("assets/castling_square.png", m_renderer);

	// Throw exception if asset could not be loaded
	if (h_texture == nullptr)
	{
		SDL_Log("Failed to load active_piece image!!!");
		// Throw Exception
		return;
	}

	h_square = { (int) xpos, (int) ypos, (m_grid_size - p_offset),
		(m_grid_size - p_offset)};
	SDL_SetRenderTarget(m_renderer, m_chess_board);
	SDL_RenderCopy(m_renderer, h_texture, nullptr, &h_square);
	SDL_RenderPresent(m_renderer);
	SDL_SetRenderTarget(m_renderer, nullptr);
}

/**
 * highlightRoute - Highlights the route of different grid(s) a piece can move
 * to on the board
 *
 * @p: Pointer to the piece whoose route should be highlighted
 * @pinned: Boolean parameter indicating whether or not the piece to highlight
 * is pinned
 *
 * Return: Nothing
 */
void ChessBoard::highlightRoute(Piece* p, bool pinned)
{
	int x, y;

	if (!p)
		return;
	x = p->getX();
	y = p->getY();
	switch(p->getPieceType())
	{
		case PAWN:
			highlightPawnRoutes(p);
			break;
		case KNIGHT:
			highlightKnightRoutes(p, pinned);
			break;
		case BISHOP:
			highlightDiagonal(p, pinned);
			break;
		case ROOK:
			highlightStraight(p, pinned);
			break;
		case QUEEN:
			highlightDiagonal(p, pinned);
			highlightStraight(p, pinned);
			break;
		case KING:
			highlightKingRoutes(p);
			break;
		default:
			break;
	}
}

/**
 * highlightInterceptRoute - Highlights the possible intercept grid(s) if any
 * for a piece to protect his king under check
 *
 * @p: Pointer to the piece whoose route should be highlighted
 *
 * Return: Nothing
 */
void ChessBoard::highlightInterceptRoute(Piece* p)
{
	King* king;
	PieceType type;

	if (!p)
		return;

	type = p->getPieceType();
	if (type == KING)
		return;

	king = p->isBlack() ? m_black_king : m_white_king;
	if (type == PAWN || type == ROOK)
	{
		Grid tmp;

		if (type == PAWN)
			tmp = dynamic_cast<Pawn*>(p)->getIntercept();
		else
			tmp = dynamic_cast<Rook*>(p)->getIntercept();

		if (tmp.getX() >= 0 && tmp.getY() >= 0)
			highlight(tmp.getX(), tmp.getY(), MOVE);
	} else
	{
		std::vector<Grid> tmp_v;

		switch (type)
		{
			case QUEEN:
				tmp_v = dynamic_cast<Queen*>(p)->getIntercepts();
				break;
			case BISHOP:
				tmp_v = dynamic_cast<Bishop*>(p)->getIntercepts();
				break;
			case KNIGHT:
				tmp_v = dynamic_cast<Knight*>(p)->getIntercepts();
				break;
			default:
				break;
		}
		for (int i = 0; i < tmp_v.size(); i++)
			highlight(tmp_v[i].getX(), tmp_v[i].getY(), MOVE);
	}
}

/**
 * highlightKingEvade - Highlights squares a king can go to to evade a check
 *
 * Return: Nothing
 */
void ChessBoard::highlightKingEvade()
{
	int x, y;
	King* king;
	std::vector<Grid> safe_grids;

	king = m_black_turn ? m_black_king : m_white_king;
	if (!king->getAttacker())
		return;
	x = king->getX();
	y = king->getY();
	// Right
	if (x <= 6 && king->canMove(x + 1, y))
		safe_grids.push_back(Grid(x + 1, y));
	// Left
	if (x >= 1 && king->canMove(x - 1, y))
		safe_grids.push_back(Grid(x - 1, y));
	// Down
	if (y >= 1 && king->canMove(x, y - 1))
		safe_grids.push_back(Grid(x, y - 1));
	// Up
	if (y <= 6 && king->canMove(x, y + 1))
		safe_grids.push_back(Grid(x, y + 1));
	// Upper Right
	if (y <= 6 && x <= 6 && king->canMove(x + 1, y + 1))
		safe_grids.push_back(Grid(x + 1, y + 1));
	// Upper Left
	if (y <= 6 && x >= 1 && king->canMove(x - 1, y + 1))
		safe_grids.push_back(Grid(x - 1, y + 1));
	// Lower Right
	if (y >= 1 && x <= 6 && king->canMove(x + 1, y - 1))
		safe_grids.push_back(Grid(x + 1, y - 1));
	// Lower Left
	if (y >= 1 && x >= 1 && king->canMove(x - 1, y - 1))
		safe_grids.push_back(Grid(x - 1, y - 1));
	for (int i = 0; i < safe_grids.size(); i++)
	{
		int x, y;

		x = safe_grids[i].getX();
		y = safe_grids[i].getY();
		if (m_board[x][y])
			highlight(x, y, CAPTURE);
		else
			highlight(x, y, MOVE);
	}
}
