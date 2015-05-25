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
 * Represents a CheckersBoard, including all the sqaures and the pieces on those squares.
 * Defines operations for moving, get and setting pieces on square.
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

	// The default starting positions of all the pieces.
	static const PieceType DefaultPieceLayout[NumberOfSquares];

	/**
	 * Creates a new board with the default piece layout.
	*/
	CheckersBoard() : CheckersBoard(DefaultPieceLayout, SideType::White) {}

    CheckersBoard( const PieceType pieceTypes[NumberOfSquares], SideType currentSide );

    PieceType GetPiece( Pos pos ) {
        if( IsOutOfBounds(pos) ) return PieceType::None;
        return m_pieces[PosToIndex(pos)];
    }

    void SetPiece( Pos pos, PieceType piece ) {
        assert( !IsOutOfBounds(pos) );
        m_pieces[PosToIndex(pos)] = piece;
    }

    bool IsOccupied( Pos pos ) {
        // An out of bounds pos is considered occupied
        return IsOutOfBounds(pos) || PieceType::None != m_pieces[PosToIndex(pos)];
    }

    bool IsOutOfBounds( Pos pos ) {
        return  pos.row < 0 ||
                pos.row >= NumberOfRows ||
                pos.column < 0 ||
                pos.column >= NumberOfColumns;
    }

    bool CanMove( Move move ) {
        return GetMoveError(move)==MoveError::None;
    }

    MoveError GetMoveError( Move move )
    {
        if( !IsOccupied(move.from) ) return MoveError::NoPieceToMove;
        if( IsOccupied(move.to) ) return MoveError::IsOccupied;
        if( IsOutOfBounds(move.to) ) return MoveError::IsOutOfBounds;
        if( !move.from.IsDiagonal(move.to) ) return MoveError::IsNotDiagonal;

        int dist = move.from.GetDistance(move.to);
        if( dist==2 )
        {
            return MoveError::None;
        }
        else if( dist==4 )
        {
            Pos jumpPos = move.from + (move.to-move.from).Clamp1();
            return IsOccupied(jumpPos) ? MoveError::None : MoveError::NoJumpPiece;
        }
        return MoveError::TooFar;
    }

    void Move( Move move ) {
        if( !CanMove(move) ) { assert(false); return; }
        PieceType piece = GetPiece( move.from );
        SetPiece( move.from, PieceType::None );
        SetPiece( move.to, piece );
        m_currentSide = m_currentSide==SideType::White ? SideType::Black : SideType::Black;
    }

private:
    int PosToIndex( Pos pos ) { return (pos.row*NumberOfColumns) + pos.column; }

    SideType m_currentSide;

	PieceType m_pieces[NumberOfSquares];
};

}
