#include "AIPlayer.h"
#include "CheckersBoard.h"

#include "gtest/gtest.h"

using namespace checkers;

static const Piece::PieceType EmptyPieceLayout[CheckersBoard::NumberOfSquares] {};


TEST( ai_player, test_get_only_move )
{
	CheckersBoard board(EmptyPieceLayout, CheckersBoard::SideType::White);

	Pos p1{ 0, 0 };
	Pos p2{ 1, 1 };

	board.SetPiece(p1, Piece::PieceType::White);

	AIPlayer aiPlayer;
	Move move = aiPlayer.ChooseBestMove(board);

	EXPECT_EQ(move.to, p2);
}

TEST(ai_player, test_get_jump_move)
{
	CheckersBoard board(EmptyPieceLayout, CheckersBoard::SideType::White);

	Pos p1{ 0, 0 };
	Pos p2{ 1, 1 }; // p1 can jump p2
	Pos p3{ 4, 0 };

	board.SetPiece(p1, Piece::PieceType::White);
	board.SetPiece(p2, Piece::PieceType::Black);
	board.SetPiece(p3, Piece::PieceType::White);

	AIPlayer aiPlayer;
	Move move = aiPlayer.ChooseBestMove(board);

	Pos endPos{ 2, 2 };
	EXPECT_EQ( move.to, endPos );
}
