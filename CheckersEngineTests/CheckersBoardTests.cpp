#include "CheckersBoard.h"

#include "gtest/gtest.h"

using namespace checkers;
using PieceType = checkers::Piece::PieceType;

static const PieceType EmptyPieceLayout[CheckersBoard::NumberOfSquares] {};


class EmptyBoardTest : public ::testing::Test
{
public:
    EmptyBoardTest() :
        board( EmptyPieceLayout, CheckersBoard::SideType::White )
    {}

    CheckersBoard board;
};


class DefaultBoardTest : public ::testing::Test
{
public:
    CheckersBoard board;
};


TEST_F( DefaultBoardTest, test_is_occupied )
{
    EXPECT_FALSE( board.IsOccupied( {0, 0} ) ); // Lower left square is never occupied
    EXPECT_TRUE( board.IsOccupied( {0, 1} ) ); // The first white piece

    // Out of bounds
    EXPECT_TRUE( board.IsOccupied( { -1, 0} ) );
    EXPECT_TRUE( board.IsOccupied( {CheckersBoard::NumberOfRows, 0} ) );
    EXPECT_TRUE( board.IsOccupied( {0, CheckersBoard::NumberOfColumns} ) );
}


TEST_F( DefaultBoardTest, test_is_out_of_bounds )
{
    EXPECT_TRUE( board.IsOutOfBounds( {0, -1} ) );
}

TEST_F( DefaultBoardTest, test_get_piece )
{
    EXPECT_EQ( board.GetPiece( {0, 0} ).pieceType, PieceType::None );
    EXPECT_EQ( board.GetPiece( {0, 1} ).pieceType, PieceType::White );
    EXPECT_EQ( board.GetPiece( {0, -1} ).pieceType, PieceType::None ); // Out of bounds
}


TEST_F( DefaultBoardTest, test_move )
{
    Move move = {{2, 1}, {3, 0}};

    EXPECT_TRUE( board.IsOccupied( move.from ) );
    EXPECT_FALSE( board.IsOccupied( move.to ) );

    board.DoMove( move );

    EXPECT_FALSE( board.IsOccupied( move.from ) );
    EXPECT_TRUE( board.IsOccupied( move.to ) );
}

TEST_F(EmptyBoardTest, test_jump_removes_piece)
{
	Pos startPos{ 0, 0 };
	Pos p1{ 1, 1 };
	Pos endPos{ 2, 2 };

	board.SetPiece(startPos, PieceType::White);
	board.SetPiece(p1, PieceType::Black);

	EXPECT_EQ(board.GetPiece(p1).pieceType, PieceType::Black);
	board.DoMove(Move{ startPos, endPos });
	EXPECT_EQ(board.GetPiece(p1).pieceType, PieceType::None); // Piece is taken
}

TEST_F( DefaultBoardTest, test_can_move_one_diagonal )
{
    EXPECT_EQ( board.GetMoveError( { { 2, 1 }, { 3, 0 } } ), CheckersBoard::MoveError::None );
    EXPECT_EQ( board.GetMoveError( { { 1, 2 }, { 4, 0 } } ), CheckersBoard::MoveError::IsNotDiagonal );
}

TEST_F( DefaultBoardTest, test_cant_move_occupied )
{
    EXPECT_FALSE( board.CanMove( {{2, 1}, {1, 2}} ) );
}

TEST_F( DefaultBoardTest, test_cant_move_out_of_bounds )
{
    EXPECT_FALSE( board.CanMove( {{0, 1}, { -1, 0}} ) );
}

TEST_F( EmptyBoardTest, test_cant_move_no_piece )
{
    EXPECT_EQ( board.GetMoveError( { {0, 0}, {1, 1} } ), CheckersBoard::MoveError::NoPieceToMove );
}


TEST_F( EmptyBoardTest, test_can_move_error_jump )
{
    board.SetPiece( {0, 0}, PieceType::White );
    board.SetPiece( {1, 1}, PieceType::Black );
    EXPECT_EQ( board.GetMoveError( { {0, 0}, {2, 2} } ), CheckersBoard::MoveError::None );

    board.RemovePiece( {1, 1} );
    EXPECT_EQ( board.GetMoveError( { {0, 0}, {2, 2} } ), CheckersBoard::MoveError::NoJumpPiece );
}

TEST_F( DefaultBoardTest, test_can_move_error_jump_default_board )
{
    std::vector<Move> jumpMoves;
    board.GetJumpMoves( jumpMoves );
    EXPECT_EQ( 0, jumpMoves.size() );

    EXPECT_EQ( board.GetMoveError( { { 2, 1 }, { 3, 0 } } ), CheckersBoard::MoveError::None );
}

TEST_F( EmptyBoardTest, test_cant_move_wrong_side )
{
    Pos p1{ 0, 0 };
    Pos p2{ 1, 1 };

    board.SetPiece( p1, PieceType::White );
    Move move{ p1, p2 };

    // White's turn
    EXPECT_EQ( board.GetCurrentSide(), CheckersBoard::SideType::White );
    EXPECT_EQ( board.GetMoveError( move ), CheckersBoard::MoveError::None );
    board.DoMove( move ); // Changes the side

    // Black's turn
    EXPECT_EQ( board.GetCurrentSide(), CheckersBoard::SideType::Black );

    board.SetPiece( p1, PieceType::White );
    board.RemovePiece( p2 );

    // Can't move the white piece
    EXPECT_EQ( board.GetMoveError( move ), CheckersBoard::MoveError::WrongSide );
}

TEST_F( EmptyBoardTest, test_cant_move_backward )
{
    Pos startPos{ 1, 1 };
    board.SetPiece( startPos, PieceType::White );

    Move moveForward{ startPos, startPos + Pos{ 1, 1 } };
    EXPECT_EQ( board.GetMoveError( moveForward ), CheckersBoard::MoveError::None );

    Move moveBackward{ startPos, startPos - Pos{ 1, 1 } };
    EXPECT_EQ( board.GetMoveError( moveBackward ), CheckersBoard::MoveError::IsBackwards );
}

TEST_F( EmptyBoardTest, test_jumps_available )
{
    std::vector<Move> jumpMoves;

    // No jump moves available
    board.GetJumpMoves( jumpMoves );
    EXPECT_EQ( 0, jumpMoves.size() );

    board.SetPiece( { 0, 0 }, PieceType::White );
    board.GetJumpMoves( jumpMoves );
    EXPECT_EQ( 0, jumpMoves.size() );

    board.SetPiece( { 1, 1 }, PieceType::Black );
    board.GetJumpMoves( jumpMoves );
    EXPECT_EQ( 1, jumpMoves.size() );
    jumpMoves.clear();

    board.SetPiece( { 2, 2 }, PieceType::Black );
    board.GetJumpMoves( jumpMoves );
    EXPECT_EQ( 0, jumpMoves.size() );
}

TEST_F( EmptyBoardTest, test_cant_move_if_jump_available )
{
    Pos startPos{ 0, 0 };
    board.SetPiece( startPos, PieceType::White );

    Pos jumpPos{ 1, 1 };
    board.SetPiece( jumpPos, PieceType::Black );

    // Jump move is OK
    Move jumpMove{ startPos, startPos + Pos{ 2, 2 } };
    EXPECT_EQ( board.GetMoveError( jumpMove ), CheckersBoard::MoveError::None );

    Pos otherPos{ 4, 4 };
    board.SetPiece( otherPos, PieceType::White );

    Move otherMove{ { 4, 4 }, { 5, 5 } };
    EXPECT_EQ( board.GetMoveError( otherMove ), CheckersBoard::MoveError::MustJump );
}

TEST_F( EmptyBoardTest, test_multi_jump )
{
    Pos p1{ 0, 0 };
    Pos jumpPos1{ 1, 1 };
    Pos p2{ 2, 2 };
    Pos jumpPos2{ 3, 3 };
    Pos p3{ 4, 4 };

    // Set up a double jump
    board.SetPiece( p1, PieceType::White );
    board.SetPiece( jumpPos1, PieceType::Black );
    board.SetPiece( jumpPos2, PieceType::Black );

    EXPECT_EQ( board.GetCurrentSide(), CheckersBoard::SideType::White );
    board.DoMove( Move{ p1, p2 } ); // Wont swap sides

    EXPECT_EQ( board.GetCurrentSide(), CheckersBoard::SideType::White );
    board.DoMove( Move{ p2, p3 } ); // Will swap sides, no more jumps

    EXPECT_EQ( board.GetCurrentSide(), CheckersBoard::SideType::Black );
}

TEST_F( EmptyBoardTest, test_make_a_king )
{
    Pos p1{ CheckersBoard::NumberOfRows-2, 0 };
    Pos p2{ CheckersBoard::NumberOfRows-1, 1 };

    board.SetPiece(p1, PieceType::White);
    EXPECT_FALSE( board.GetPiece(p1).isKing );
    board.DoMove( Move { p1, p2 } );
    EXPECT_TRUE( board.GetPiece(p2).isKing );
}

TEST_F( EmptyBoardTest, test_king_moving_backwards )
{
	Move backwardsMove{ Pos{ 1, 1 }, Pos{ 0, 0 } };

	// Place a plain piece
	board.SetPiece(backwardsMove.from, Piece(PieceType::White, false));
	EXPECT_EQ(board.GetMoveError(backwardsMove), CheckersBoard::MoveError::IsBackwards);

	// Place a king piece
	board.SetPiece(backwardsMove.from, Piece(PieceType::White, true));
	EXPECT_EQ(board.GetMoveError(backwardsMove), CheckersBoard::MoveError::None);
}

TEST_F( EmptyBoardTest, test_king_jump )
{
	Move backwardsJump{ Pos{ 2, 2 }, Pos{ 0, 0 } };

	board.SetPiece(backwardsJump.from, Piece(PieceType::White, false)); // Plain piece
	board.SetPiece(backwardsJump.GetJumpPos(), PieceType::Black);

	EXPECT_EQ(board.GetMoveError(backwardsJump), CheckersBoard::MoveError::IsBackwards);

	board.SetPiece(backwardsJump.from, Piece(PieceType::White, true)); // King
	EXPECT_EQ(board.GetMoveError(backwardsJump), CheckersBoard::MoveError::None);

	board.DoMove(backwardsJump);
	EXPECT_EQ(board.GetPiece(backwardsJump.GetJumpPos()).pieceType, PieceType::None);
}

TEST_F(EmptyBoardTest, test_win)
{
	EXPECT_TRUE(board.IsFinished());

	board.SetPiece(Pos{ 0, 0 }, PieceType::Black);
	board.SetPiece(Pos{ 1, 1 }, PieceType::White);

	EXPECT_FALSE(board.IsFinished());
	board.RemovePiece(Pos{ 1, 1 });

	EXPECT_TRUE(board.IsFinished()); // No white pieces, Black is the winner.
	EXPECT_EQ(board.GetWinner(), CheckersBoard::WinType::Black);
}

TEST_F(DefaultBoardTest, test_get_moves_default)
{
	std::vector<Move> moves;
	board.GetMoves(moves);
	EXPECT_FALSE(moves.empty());
}

TEST_F(EmptyBoardTest, test_get_moves_one)
{
	Pos p1{ 0, 0 };
	board.SetPiece(p1, PieceType::White);

	std::vector<Move> moves;
	board.GetMoves(moves);

	EXPECT_EQ(1, moves.size());
	EXPECT_TRUE(board.CanMove(moves[0]));
}

TEST_F(EmptyBoardTest, test_get_moves_two)
{
	CheckersBoard board(EmptyPieceLayout, CheckersBoard::SideType::White);

	Pos p1{ 0, 0 };
	Pos p2{ 0, 2 };
	Pos p3{ 1, 1 }; // p1 and p2 can jump p2
	Pos p4{ 4, 0 }; // Can't move, jumps happen first

	board.SetPiece(p1, Piece::PieceType::White);
	board.SetPiece(p2, Piece::PieceType::White);
	board.SetPiece(p3, Piece::PieceType::Black);
	board.SetPiece(p4, Piece::PieceType::White);

	std::vector<Move> moves;
	board.GetMoves(moves);

	EXPECT_EQ(2, moves.size());
	EXPECT_EQ(board.GetMoveError(moves[0]), CheckersBoard::MoveError::None);
	EXPECT_EQ(board.GetMoveError(moves[1]), CheckersBoard::MoveError::None);
}

TEST_F(DefaultBoardTest, test_copy_constructor)
{
	board.SetPiece({ 0, 0 }, PieceType::White);
	CheckersBoard board1(board);
	board1.DoMove(Move{ { 0, 0 }, { 1, 1 } });

	EXPECT_NE(board.GetPiece({ 1, 1 }).pieceType, PieceType::White);
	EXPECT_EQ(board1.GetPiece({ 1, 1 }).pieceType, PieceType::White);
}
