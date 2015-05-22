#include "CheckersBoard.h"

using namespace checkers;

CheckersBoard::CheckersBoard( PieceType pieceTypes[NumberOfSquares] ) :
    m_currentSide(SideType::White)
{
    for( int i=0; i<NumberOfSquares; i++ )
    {
        m_pieces[i] = pieceTypes[i];
    }
}

