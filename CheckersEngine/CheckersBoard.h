#pragma once

#include "Pos.h"

#include <assert.h>

namespace checkers {

/**
 * Defines a move from one square to another.
 */
struct Move
{
    Pos from;
    Pos to;
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
        NoJumpPiece, TooFar, NoPieceToMove, WrongSide };

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
    void SetPiece( Pos pos, PieceType piece )
	{
		if ( IsOutOfBounds(pos) ) { assert(false && "Piece out of bounds"); return; }
        m_pieces[PosToIndex(pos)] = piece;
    }

	/// Returns whether the position is occupied. An out of bounds pos is considered occupied.
	bool IsOccupied(Pos pos) const
	{
        return IsOutOfBounds(pos) || PieceType::None != m_pieces[PosToIndex(pos)];
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

	/// Verifys a move and returns the error, can be MoveError::None.
	MoveError GetMoveError(const Move& move) const;

	/// Performs the move
	void DoMove(Move move);

private:
	/// Convert a Pos into an index into the board array.
    int PosToIndex( Pos pos ) const { return (pos.row*NumberOfColumns) + pos.column; }

	/// The current side whose turn it is to move.
    SideType m_currentSide;

	/// The board array.
	PieceType m_pieces[NumberOfSquares];

	// The default starting positions of all the pieces.
	static const PieceType DefaultPieceLayout[NumberOfSquares];

};

}
