#include "AIPlayer.h"
#include "CheckersBoard.h"

#include "gtest/gtest.h"

using namespace checkers;

static const Piece::PieceType EmptyPieceLayout[CheckersBoard::NumberOfSquares] {};


TEST( ai_player, test_get_move )
{
	CheckersBoard board(EmptyPieceLayout, CheckersBoard::SideType::White);

	Pos p1{ 0, 0 };
	Pos p2{ 1, 1 };

	board.SetPiece(p1, Piece::PieceType::White);

	AIPlayer aiPlayer;
	Move move = aiPlayer.ChooseBestMove(board);

	EXPECT_EQ(move.to, p2);
}
