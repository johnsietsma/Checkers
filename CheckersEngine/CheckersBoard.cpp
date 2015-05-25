#include "CheckersBoard.h"

#include <algorithm>
#include <cstring> // For memcpy

using namespace checkers;


const CheckersBoard::PieceType CheckersBoard::DefaultPieceLayout[NumberOfSquares] = {
    PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White,
    PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,
    PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White, PieceType::None,  PieceType::White,
    PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,
    PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,  PieceType::None,
    PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,
    PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black,
    PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None,  PieceType::Black, PieceType::None
};

CheckersBoard::CheckersBoard( const PieceType pieceTypes[NumberOfSquares], SideType startSide ) :
    m_currentSide( startSide )
{
    std::memcpy( m_pieces, pieceTypes, sizeof( m_pieces ) );
}


std::vector<Move> CheckersBoard::GetJumpMoves() const
{
    static const Pos jumpDeltas[4] { Pos{ 2, 2 }, Pos{ -2, 2 }, Pos{ -2, -2 }, Pos{ 2, -2 } };
    std::vector<Move> jumpMoves;
    for ( int row = 0; row < NumberOfRows; row++ ) {
        for ( int column = 0; column < NumberOfColumns; column++ ) {
            for ( auto jumpDelta : jumpDeltas ) {
                Pos startPos{ row, column };
                Move jumpMove{ startPos, startPos + jumpDelta };
                if ( GetMoveError_DontForceJumps( jumpMove ) == MoveError::None ) {
                    jumpMoves.push_back( jumpMove );
                }
            }
        }
    }
    return jumpMoves;
}


CheckersBoard::MoveError CheckersBoard::GetMoveError( const checkers::Move &move ) const
{
    auto moveError = GetMoveError_DontForceJumps( move );
    if ( moveError == MoveError::None ) {
        // Check the available jump separately to the move errors to avoid recursion
        auto jumpMoves = GetJumpMoves();
        if ( jumpMoves.size() > 0 && std::find( std::begin( jumpMoves ), std::end( jumpMoves ), move ) == std::end( jumpMoves ) ) {
            return MoveError::MustJump;
        }
    }
    return moveError;
}

CheckersBoard::MoveError CheckersBoard::GetMoveError_DontForceJumps( const checkers::Move &move ) const
{
    // Basic checks
    if ( !IsOccupied( move.from ) ) { return MoveError::NoPieceToMove; }
    if ( IsOccupied( move.to ) ) { return MoveError::IsOccupied; }
    if ( IsOutOfBounds( move.to ) ) { return MoveError::IsOutOfBounds; }
    if ( !move.from.IsDiagonal( move.to ) ) { return MoveError::IsNotDiagonal; }

    // Check we're moving the right color piece
    PieceType pieceType = GetPiece( move.from );
    if ( pieceType == PieceType::White && GetCurrentSide() != SideType::White ) { return MoveError::WrongSide; }
    if ( pieceType == PieceType::Black && GetCurrentSide() != SideType::Black ) { return MoveError::WrongSide; }

    // Check for backwards move
    int forwardDist = move.to.row - move.from.row;
    if ( forwardDist > 0 && pieceType == PieceType::Black ) { return MoveError::IsBackwards; }
    if ( forwardDist < 0 && pieceType == PieceType::White ) { return MoveError::IsBackwards; }


    int dist = move.from.GetDistance( move.to );
    if ( dist == 2 ) {
        // We're moving to an adjacent diagonal sqaure
        return MoveError::None;
    }
    else if ( dist == 4 ) {
        // We're jumping, check for an appropriate piece to jump
        Pos jumpPos = move.from + ( move.to - move.from ).Clamp1();
        return IsOccupied( jumpPos, GetOpponentPieceType( pieceType ) ) ? MoveError::None : MoveError::NoJumpPiece;
    }

    // We're moving some other distance, not allowed
    return MoveError::TooFar;
}

void CheckersBoard::DoMove( const Move &move )
{
    if ( !CanMove( move ) ) { throw std::out_of_range( "Move is not allowed" ); }
    PieceType piece = GetPiece( move.from );
    SetPiece( move.from, PieceType::None );
    SetPiece( move.to, piece );
    m_currentSide = m_currentSide == SideType::White ? SideType::Black : SideType::Black;
}

