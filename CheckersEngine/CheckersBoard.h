#pragma once

#include "Pos.h"

#include <assert.h>
#include <vector>

namespace checkers {

/**
 * Defines a move from one square to another.
 */
struct Move
{
    Pos from;
    Pos to;

	bool operator== (const Move& rhs) const
	{
		return from == rhs.from && to == rhs.to;
	}

	bool operator!= (const Move& rhs) const
	{
		return !(*this == rhs);
	}
};

/**
 * Represents a logical CheckersBoard, including all the sqaures and the pieces on those squares.
 * Defines operations for moving, get and setting pieces on square.
 * Checks move errors and performs moves.
 */
class CheckersBoard
{
public:
    enum class PieceType { None, White, Black };
    enum class SideType { White, Black };
    enum class MoveError { None, IsOutOfBounds, IsOccupied, IsNotDiagonal, IsNotAdjacent,
        NoJumpPiece, TooFar, NoPieceToMove, WrongSide, IsBackwards, MustJump };

    const static int NumberOfSquares = 64;
    const static int NumberOfColumns = 8;
    const static int NumberOfRows = 8;

	/// Creates a new board with the default piece layout.
	CheckersBoard() : CheckersBoard(DefaultPieceLayout, SideType::White) {}

	/// Create a board with a custom layout and starting side.
    CheckersBoard( const PieceType pieceTypes[NumberOfSquares], SideType currentSide );

	SideType GetCurrentSide() const { return m_currentSide;  }

	/// Get the piece at the given position. Returns PieceType::None if ot of bounds.
    PieceType GetPiece( Pos pos ) const
	{
        if( IsOutOfBounds(pos) ) return PieceType::None;
        return m_pieces[PosToIndex(pos)];
    }

	/// Sets a piece at a given location.
	// This is only public for testing purposes. use DoMove() instead.
    void SetPiece( Pos pos, PieceType piece )
	{
		if ( IsOutOfBounds(pos) ) { assert(false && "Piece out of bounds"); return; }
        m_pieces[PosToIndex(pos)] = piece;
    }

	/// Get all the jump moves that the current size can make.
	std::vector<Move> GetJumpMoves() const;

	/// Returns whether the position is occupied. An out of bounds pos is considered occupied.
	bool IsOccupied(Pos pos) const
	{
		return IsOccupied(pos, PieceType::Black) || IsOccupied(pos, PieceType::White);
    }

	// Returns whether a square is occupied by a particular PieceType.
	bool IsOccupied(Pos pos, PieceType pieceType) const
	{
		return IsOutOfBounds(pos) || pieceType == m_pieces[PosToIndex(pos)];
	}

	/// Returns whether a position is out of bounds.
    bool IsOutOfBounds( Pos pos ) const
	{
        return  pos.row < 0 ||
                pos.row >= NumberOfRows ||
                pos.column < 0 ||
                pos.column >= NumberOfColumns;
    }

	/// Returns whether a move can be made.
    bool CanMove( Move move ) const
	{
        return GetMoveError(move)==MoveError::None;
    }

	/// Verifies a move and returns the error, can be MoveError::None.
	MoveError GetMoveError(const Move& move) const;

	/// Performs the move
	void DoMove(Move move);

private:
	/// Convert a Pos into an index into the board array.
    static int PosToIndex( Pos pos ) { return (pos.row*NumberOfColumns) + pos.column; }

	/// Return the opponents piece type.
	static PieceType GetOpponentPieceType(PieceType pieceType)  { return pieceType == PieceType::White ? PieceType::Black : PieceType::White; }

	/// Checks for all move errors, but doesn't return an error if there is a jump available.
	MoveError GetMoveError_DontForceJumps(const Move& move) const;

	/// The current side whose turn it is to move.
    SideType m_currentSide;

	/// The board array.
	PieceType m_pieces[NumberOfSquares];

	// The default starting positions of all the pieces.
	static const PieceType DefaultPieceLayout[NumberOfSquares];

};

}
