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
        m_pieces[i] = Piece(pieceTypes[i], false);
    }
}

CheckersBoard::CheckersBoard(const CheckersBoard& board)
{
	memcpy(m_pieces, board.m_pieces, sizeof(m_pieces));
	m_currentSide = board.m_currentSide;
}

void CheckersBoard::GetMoves(std::vector<Move> &moves) const
{
	int currentMoveCount = moves.size();
	for (int row = 0; row < NumberOfRows; row++) {
		for (int column = 0; column < NumberOfColumns; column++) {
			Pos startPos{ row, column };
			GetJumpMoves(startPos, moves);
		}
	}

	// If there are jump moves, we have to do them first.
	if (currentMoveCount != moves.size()) return;

	for (int row = 0; row < NumberOfRows; row++) {
		for (int column = 0; column < NumberOfColumns; column++) {
			Pos startPos{ row, column };
			GetMoves(startPos, moves); 
		}
	}
}

void CheckersBoard::GetMoves(const Pos &pos, std::vector<Move> &moves) const
{
	static const Pos moveDeltas[4] { Pos{ 1, 1 }, Pos{ -1, 1 }, Pos{ -1, -1 }, Pos{ 1, -1 } };
	GetMoves(pos, moveDeltas, moves);
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
	GetMoves(startPos, jumpDeltas, jumpMoves);
}

void CheckersBoard::GetMoves(const Pos &startPos, const Pos moveDeltas[4], std::vector<Move> &moves) const
{
	for (int i = 0; i < 4; i++) {
		auto moveDelta = moveDeltas[i];
		Move move{ startPos, startPos + moveDelta };
		if (GetMoveError_DontForceJumps(move) == MoveError::None) {
			moves.push_back(move);
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
    Piece piece = GetPiece( move.from );
	PieceType pieceType = piece.pieceType;
    if ( pieceType == PieceType::White && GetCurrentSide() != SideType::White ) { return MoveError::WrongSide; }
    if ( pieceType == PieceType::Black && GetCurrentSide() != SideType::Black ) { return MoveError::WrongSide; }

    // Check for backwards move
    int forwardDist = move.to.row - move.from.row;
    if ( !piece.isKing && forwardDist > 0 && pieceType == PieceType::Black ) { return MoveError::IsBackwards; }
    if ( !piece.isKing && forwardDist < 0 && pieceType == PieceType::White ) { return MoveError::IsBackwards; }

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

	// Check for king making
    if( IsOnLastRow(piece.pieceType, move.to) ) {
        piece.isKing = true;
    }

	// Do the actual move
    RemovePiece( move.from );
    SetPiece( move.to, piece );

	// Check for jump
	if (move.IsJumpMove())
	{
		// Remove the captured piece
		auto jumpPos = move.GetJumpPos();
		RemovePiece(jumpPos);
	}

    // Only swap sides if we can't jump again from our new Pos
    std::vector<Move> jumpMoves;
    GetJumpMoves( move.to, jumpMoves );
	if ( jumpMoves.empty() )
    {
		m_currentSide = GetCurrentOpponentSide();
    }
}
