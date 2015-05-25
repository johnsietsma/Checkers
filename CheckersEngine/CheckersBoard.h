#pragma once

#include <assert.h>
#include <vector>

#include "Move.h"

namespace checkers
{

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
                           NoJumpPiece, TooFar, NoPieceToMove, WrongSide, IsBackwards, MustJump
                         };

    const static int NumberOfSquares = 64;
    const static int NumberOfColumns = 8;
    const static int NumberOfRows = 8;

    /// Creates a new board with the default piece layout.
    CheckersBoard() : CheckersBoard( DefaultPieceLayout, SideType::White ) {}

    /// Create a board with a custom layout and starting side.
    CheckersBoard( const PieceType pieceTypes[NumberOfSquares], SideType currentSide );

    SideType GetCurrentSide() const { return m_currentSide;  }

    /// Get the piece at the given position. Returns PieceType::None if ot of bounds.
    PieceType GetPiece( const Pos &pos ) const;

    /// Sets a piece at a given location.
    // This is only public for testing purposes. use DoMove() instead.
    void SetPiece( const Pos &pos, PieceType piece );

    /// Get all the jump moves that the current size can make.
    std::vector<Move> GetJumpMoves() const;

    /// Returns whether the position is occupied. An out of bounds pos is considered occupied.
    bool IsOccupied( const Pos &pos ) const;

    // Returns whether a square is occupied by a particular PieceType.
    bool IsOccupied( const Pos &pos, PieceType pieceType ) const;

    /// Returns whether a position is out of bounds.
    bool IsOutOfBounds( const Pos &pos ) const;

    /// Returns whether a move can be made.
    bool CanMove( const Move &move ) const;

    /// Verifies a move and returns the error, can be MoveError::None.
    MoveError GetMoveError( const Move &move ) const;

    /// Performs the move
    void DoMove( const Move &move );

private:
    /// Convert a Pos into an index into the board array.
    static int PosToIndex( const Pos &pos ) { return ( pos.row * NumberOfColumns ) + pos.column; }

    /// Return the opponents piece type.
    static PieceType GetOpponentPieceType( PieceType pieceType )  { return pieceType == PieceType::White ? PieceType::Black : PieceType::White; }

    /// Checks for all move errors, but doesn't return an error if there is a jump available.
    MoveError GetMoveError_DontForceJumps( const Move &move ) const;

    /// The current side whose turn it is to move.
    SideType m_currentSide;

    /// The board array.
    PieceType m_pieces[NumberOfSquares];

    // The default starting positions of all the pieces.
    static const PieceType DefaultPieceLayout[NumberOfSquares];

};

inline CheckersBoard::PieceType CheckersBoard::GetPiece( const Pos &pos ) const
{
    if ( IsOutOfBounds( pos ) ) { return PieceType::None; }
    return m_pieces[PosToIndex( pos )];
}

inline void CheckersBoard::SetPiece( const Pos& pos, CheckersBoard::PieceType piece )
{
    if ( IsOutOfBounds( pos ) ) { assert( false && "Piece out of bounds" ); return; }
    m_pieces[PosToIndex( pos )] = piece;
}

inline bool CheckersBoard::IsOccupied( const Pos &pos ) const
{
    return IsOccupied( pos, PieceType::Black ) || IsOccupied( pos, PieceType::White );
}

inline bool CheckersBoard::IsOccupied( const Pos &pos, PieceType pieceType ) const
{
    return IsOutOfBounds( pos ) || pieceType == m_pieces[PosToIndex( pos )];
}

inline bool CheckersBoard::IsOutOfBounds( const Pos &pos ) const
{
    return  pos.row < 0 ||
            pos.row >= NumberOfRows ||
            pos.column < 0 ||
            pos.column >= NumberOfColumns;
}

inline bool CheckersBoard::CanMove( const Move& move ) const
{
    return GetMoveError( move ) == MoveError::None;
}


}
