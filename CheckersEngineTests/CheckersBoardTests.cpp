#include "CheckersBoard.h"

#include "gtest/gtest.h"

using namespace checkers;
using PieceType = checkers::CheckersBoard::PieceType;



TEST(checkersboard_test, test_is_occupied)
{
	CheckersBoard board;

    EXPECT_FALSE( board.IsOccupied( {0,0} ) ); // Lower left square is never occupied
    EXPECT_TRUE( board.IsOccupied( {0,1} ) ); // The first white piece

    // Out of bounds
    EXPECT_TRUE( board.IsOccupied( {-1,0} ) );
    EXPECT_TRUE( board.IsOccupied( {CheckersBoard::NumberOfRows,0} ) );
    EXPECT_TRUE( board.IsOccupied( {0,CheckersBoard::NumberOfColumns} ) );
}

TEST(checkersboard_test, test_is_out_of_bounds)
{
    CheckersBoard board;
    EXPECT_TRUE( board.IsOutOfBounds( {0,-1} ) );
}

TEST(checkersboard_test, test_get_piece)
{
	CheckersBoard board;

    EXPECT_EQ( board.GetPiece( {0,0} ), PieceType::None );
    EXPECT_EQ( board.GetPiece( {0,1} ), PieceType::White );
    EXPECT_EQ( board.GetPiece( {0,-1} ), PieceType::None ); // Out of bounds
}


TEST(checkersboard_test, test_move)
{
	CheckersBoard board;

    Move move = {{1,2}, {3,0}};

    EXPECT_TRUE( board.IsOccupied( move.from ) );
    EXPECT_FALSE( board.IsOccupied( move.to ) );

	board.DoMove(move);

	EXPECT_FALSE( board.IsOccupied( move.from ) );
    EXPECT_TRUE( board.IsOccupied( move.to ) );
}

TEST(checkersboard_test, test_can_move_one_diagonal)
{
	CheckersBoard board;

    EXPECT_EQ( board.GetMoveError( { {2,1}, {3,0} } ), CheckersBoard::MoveError::None );
    EXPECT_EQ( board.GetMoveError( { {1,2}, {4,0} } ), CheckersBoard::MoveError::IsNotDiagonal );
}

TEST(checkersboard_test, test_cant_move_occupied)
{
    CheckersBoard board;

    EXPECT_FALSE( board.CanMove( {{2,1}, {1,2}} ) );
}

TEST(checkersboard_test, test_cant_move_out_of_bounds)
{
    CheckersBoard board;

    EXPECT_FALSE( board.CanMove( {{0,1}, {-1,0}} ) );
}

TEST(checkersboard_test, test_cant_move_no_piece)
{
	PieceType initialPieces[64] {};
    CheckersBoard board(initialPieces, CheckersBoard::SideType::White);
    EXPECT_EQ( board.GetMoveError( { {0,0}, {1,1} } ), CheckersBoard::MoveError::NoPieceToMove );
}


TEST(checkersboard_test, test_can_move_jump)
{
	PieceType initialPieces[64] {};
    CheckersBoard board(initialPieces, CheckersBoard::SideType::White);

    board.SetPiece( {0,0}, PieceType::White );
    board.SetPiece( {1,1}, PieceType::Black );
    EXPECT_EQ( board.GetMoveError( { {0,0}, {2,2} } ), CheckersBoard::MoveError::None );

    board.SetPiece( {1,1}, PieceType::None );
    EXPECT_EQ( board.GetMoveError( { {0,0}, {2,2} } ), CheckersBoard::MoveError::NoJumpPiece );
}

TEST(checkersboard_test, test_cant_move_wrong_side)
{
	// Setup empty board
	PieceType initialPieces[64] {};
	CheckersBoard board;

	Pos p1{ 0, 0 };
	Pos p2{ 1, 1 };

	board.SetPiece(p1, PieceType::White);
	board.SetPiece(p2, PieceType::None);
	Move move{ p1, p2 };

	// White's turn
	EXPECT_EQ(board.GetCurrentSide(), CheckersBoard::SideType::White);
	EXPECT_EQ(board.GetMoveError(move), CheckersBoard::MoveError::None);
	board.DoMove(move); // Changes the side

	// Black's turn
	EXPECT_EQ(board.GetCurrentSide(), CheckersBoard::SideType::Black);

	board.SetPiece(p1, PieceType::White);
	board.SetPiece(p2, PieceType::None);

	// Can't move the white piece
    EXPECT_EQ( board.GetMoveError( move ), CheckersBoard::MoveError::WrongSide );

}

TEST(checkersboard_test, test_cant_move_not_forward)
{

}
