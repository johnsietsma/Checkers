#include "CheckersBoard.h"

#include "gtest/gtest.h"

using namespace checkers;
using PieceType = checkers::CheckersBoard::PieceType;

static const PieceType EmptyPieceLayout[CheckersBoard::NumberOfSquares] {};


class EmptyBoardTest : public ::testing::Test {
public:
	EmptyBoardTest() :
		board(EmptyPieceLayout, CheckersBoard::SideType::White)
	{}

	CheckersBoard board;
};


class DefaultBoardTest : public ::testing::Test {
public:
	CheckersBoard board;
};


TEST_F(DefaultBoardTest, test_is_occupied)
{
    EXPECT_FALSE( board.IsOccupied( {0,0} ) ); // Lower left square is never occupied
    EXPECT_TRUE( board.IsOccupied( {0,1} ) ); // The first white piece

    // Out of bounds
    EXPECT_TRUE( board.IsOccupied( {-1,0} ) );
    EXPECT_TRUE( board.IsOccupied( {CheckersBoard::NumberOfRows,0} ) );
    EXPECT_TRUE( board.IsOccupied( {0,CheckersBoard::NumberOfColumns} ) );
}

TEST_F(DefaultBoardTest, test_is_out_of_bounds)
{
    EXPECT_TRUE( board.IsOutOfBounds( {0,-1} ) );
}

TEST_F(DefaultBoardTest, test_get_piece)
{
    EXPECT_EQ( board.GetPiece( {0,0} ), PieceType::None );
    EXPECT_EQ( board.GetPiece( {0,1} ), PieceType::White );
    EXPECT_EQ( board.GetPiece( {0,-1} ), PieceType::None ); // Out of bounds
}


TEST_F(DefaultBoardTest, test_move)
{
    Move move = {{2,1}, {3,0}};

    EXPECT_TRUE( board.IsOccupied( move.from ) );
    EXPECT_FALSE( board.IsOccupied( move.to ) );

	board.DoMove(move);

	EXPECT_FALSE( board.IsOccupied( move.from ) );
    EXPECT_TRUE( board.IsOccupied( move.to ) );
}

TEST_F(DefaultBoardTest, test_can_move_one_diagonal)
{
	EXPECT_EQ(board.GetMoveError({ { 2, 1 }, { 3, 0 } }), CheckersBoard::MoveError::None);
	EXPECT_EQ(board.GetMoveError({ { 1, 2 }, { 4, 0 } }), CheckersBoard::MoveError::IsNotDiagonal);
}

TEST_F(DefaultBoardTest, test_cant_move_occupied)
{
    EXPECT_FALSE( board.CanMove( {{2,1}, {1,2}} ) );
}

TEST_F(DefaultBoardTest, test_cant_move_out_of_bounds)
{
    EXPECT_FALSE( board.CanMove( {{0,1}, {-1,0}} ) );
}

TEST_F(EmptyBoardTest, test_cant_move_no_piece)
{
    EXPECT_EQ( board.GetMoveError( { {0,0}, {1,1} } ), CheckersBoard::MoveError::NoPieceToMove );
}


TEST_F(EmptyBoardTest, test_can_move_error_jump)
{
    board.SetPiece( {0,0}, PieceType::White );
    board.SetPiece( {1,1}, PieceType::Black );
    EXPECT_EQ( board.GetMoveError( { {0,0}, {2,2} } ), CheckersBoard::MoveError::None );

    board.SetPiece( {1,1}, PieceType::None );
    EXPECT_EQ( board.GetMoveError( { {0,0}, {2,2} } ), CheckersBoard::MoveError::NoJumpPiece );
}

TEST_F(DefaultBoardTest, test_can_move_error_jump_default_board)
{
	auto jumpMoves = board.GetJumpMoves();
	EXPECT_EQ(0, jumpMoves.size());

	EXPECT_EQ(board.GetMoveError({ { 2, 1 }, { 3, 0 } }), CheckersBoard::MoveError::None);
}

TEST_F(EmptyBoardTest, test_cant_move_wrong_side)
{
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

TEST_F(EmptyBoardTest, test_cant_move_backward)
{
	Pos startPos{ 1, 1 };
	board.SetPiece(startPos, PieceType::White);

	Move moveForward{ startPos, startPos + Pos{ 1, 1 } };
	EXPECT_EQ(board.GetMoveError(moveForward), CheckersBoard::MoveError::None);
	
	Move moveBackward{ startPos, startPos - Pos{ 1, 1 } };
	EXPECT_EQ(board.GetMoveError(moveBackward), CheckersBoard::MoveError::IsBackwards);
}

TEST_F(EmptyBoardTest, test_jumps_available)
{
	// No jump moves available
	EXPECT_EQ(0, board.GetJumpMoves().size());

	board.SetPiece({ 0, 0 }, PieceType::White);
	board.SetPiece({ 1, 1 }, PieceType::Black);
	EXPECT_EQ(1, board.GetJumpMoves().size());

	board.SetPiece({ 2, 2 }, PieceType::Black);
	EXPECT_EQ(0, board.GetJumpMoves().size());
}

TEST_F(EmptyBoardTest, test_cant_move_if_jump_available)
{
	Pos startPos{ 0, 0 };
	board.SetPiece(startPos, PieceType::White);

	Pos jumpPos{ 1, 1 };
	board.SetPiece(jumpPos, PieceType::Black);

	// Jump move is OK
	Move jumpMove{ startPos, startPos + Pos{ 2, 2 } };
	EXPECT_EQ(board.GetMoveError(jumpMove), CheckersBoard::MoveError::None);

	Pos otherPos{ 4, 4 };
	board.SetPiece(otherPos, PieceType::White);

	Move otherMove{ { 4, 4 }, { 5, 5 } };
	EXPECT_EQ(board.GetMoveError(otherMove), CheckersBoard::MoveError::MustJump);
}

TEST_F(EmptyBoardTest, test_multi_jump)
{

}

TEST_F(EmptyBoardTest, test_make_a_king)
{

}

TEST_F(EmptyBoardTest, test_king_moving_backwards)
{

}

TEST_F(EmptyBoardTest, test_king_jump)
{

}

