#include "CheckersBoard.h"

#include <algorithm>
#include <cstring> // For memcpy

using namespace checkers;

using PieceType = checkers::Piece::PieceType;

const Piece::PieceType CheckersBoard::DefaultPieceLayout[NumberOfSquares] = {
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
    for( int i=0; i<NumberOfSquares; i++ )
    {
        m_pieces[i].pieceType = pieceTypes[i];
    }
}


void CheckersBoard::GetJumpMoves( std::vector<Move> &jumpMoves ) const
{
    for ( int row = 0; row < NumberOfRows; row++ ) {
        for ( int column = 0; column < NumberOfColumns; column++ ) {
            Pos startPos{ row, column };
            GetJumpMoves( startPos, jumpMoves );
        }
    }
}

void CheckersBoard::GetJumpMoves( const Pos &startPos, std::vector<Move> &jumpMoves ) const
{
    static const Pos jumpDeltas[4] { Pos{ 2, 2 }, Pos{ -2, 2 }, Pos{ -2, -2 }, Pos{ 2, -2 } };

    for ( auto jumpDelta : jumpDeltas ) {
        Move jumpMove{ startPos, startPos + jumpDelta };
        if ( GetMoveError_DontForceJumps( jumpMove ) == MoveError::None ) {
            jumpMoves.push_back( jumpMove );
        }
    }
}

CheckersBoard::MoveError CheckersBoard::GetMoveError( const checkers::Move &move ) const
{
    auto moveError = GetMoveError_DontForceJumps( move );
    if ( moveError == MoveError::None ) {
        // Check the available jump separately to the move errors to avoid recursion
        std::vector<Move> jumpMoves;
        GetJumpMoves( jumpMoves );
        if ( jumpMoves.size() > 0 && std::find( jumpMoves.begin(), jumpMoves.end(), move ) == jumpMoves.end() ) {
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
    PieceType pieceType = GetPiece( move.from ).pieceType;
    if ( pieceType == PieceType::White && GetCurrentSide() != SideType::White ) { return MoveError::WrongSide; }
    if ( pieceType == PieceType::Black && GetCurrentSide() != SideType::Black ) { return MoveError::WrongSide; }

    // Check for backwards move
    int forwardDist = move.to.row - move.from.row;
    if ( forwardDist > 0 && pieceType == PieceType::Black ) { return MoveError::IsBackwards; }
    if ( forwardDist < 0 && pieceType == PieceType::White ) { return MoveError::IsBackwards; }

    if ( move.IsAdjacentMove() ) {
        // We're moving to an adjacent diagonal sqaure
        return MoveError::None;
    }
    else if ( move.IsJumpMove() ) {
        // We're jumping, check for an appropriate piece to jump
		Pos jumpPos = move.GetJumpPos();
        return IsOccupied( jumpPos, Piece::GetOpponentPieceType( pieceType ) ) ? MoveError::None : MoveError::NoJumpPiece;
    }

    // We're moving some other distance, not allowed
    return MoveError::TooFar;
}

void CheckersBoard::DoMove( const Move &move )
{
    if ( !CanMove( move ) ) { throw std::out_of_range( "Move is not allowed" ); }
    Piece piece = GetPiece( move.from );

    if( IsOnLastRow(piece.pieceType, move.to) ) {
        piece.isKing = true;
    }

    SetPiece( move.from, PieceType::None );
    SetPiece( move.to, piece );

	if (move.IsJumpMove())
	{
		// Remove the captured piece
		auto jumpPos = move.GetJumpPos();
		SetPiece(jumpPos, PieceType::None);
	}

    // Only swap if we can't jump again
    std::vector<Move> jumpMoves;
    GetJumpMoves( jumpMoves );
    if ( std::find_if( jumpMoves.begin(), jumpMoves.end(),
            [&move]( const Move & jumpMove )
            {
                return move.to == jumpMove.from; // A jump move is available from our to Pos.
            }
        ) == jumpMoves.end()
    )
    {
        m_currentSide = m_currentSide == SideType::White ? SideType::Black : SideType::Black;
    }
}

