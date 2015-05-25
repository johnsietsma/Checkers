#include "CheckersBoard.h"

#include <cstring> // For memcpy

using namespace checkers;


const CheckersBoard::PieceType CheckersBoard::DefaultPieceLayout[NumberOfSquares] = {
	PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White,
	PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None,
	PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White, PieceType::None, PieceType::White,
	PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None,
	PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None, PieceType::None,
	PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None,
	PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black,
	PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None, PieceType::Black, PieceType::None
};

CheckersBoard::CheckersBoard( const PieceType pieceTypes[NumberOfSquares], SideType startSide ) :
    m_currentSide(startSide)
{
	std::memcpy( m_pieces, pieceTypes, sizeof(m_pieces) );
}

CheckersBoard::MoveError CheckersBoard::GetMoveError(const checkers::Move& move) const
{
	if (!IsOccupied(move.from)) return MoveError::NoPieceToMove;
	if (IsOccupied(move.to)) return MoveError::IsOccupied;
	if (IsOutOfBounds(move.to)) return MoveError::IsOutOfBounds;
	if (!move.from.IsDiagonal(move.to)) return MoveError::IsNotDiagonal;

	PieceType pieceType = GetPiece(move.from);
	if (pieceType == PieceType::White && GetCurrentSide() != SideType::White) return MoveError::WrongSide;
	if (pieceType == PieceType::Black && GetCurrentSide() != SideType::Black) return MoveError::WrongSide;

	int dist = move.from.GetDistance(move.to);
	if (dist == 2)
	{
		return MoveError::None;
	}
	else if (dist == 4)
	{
		Pos jumpPos = move.from + (move.to - move.from).Clamp1();
		return IsOccupied(jumpPos) ? MoveError::None : MoveError::NoJumpPiece;
	}
	return MoveError::TooFar;
}

void CheckersBoard::DoMove(Move move)
{
	if (!CanMove(move)) { assert(false); return; }
	PieceType piece = GetPiece(move.from);
	SetPiece(move.from, PieceType::None);
	SetPiece(move.to, piece);
	m_currentSide = m_currentSide == SideType::White ? SideType::Black : SideType::Black;
}

