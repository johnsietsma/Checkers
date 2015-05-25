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

