#pragma once


namespace checkers {

struct Piece {
    enum class PieceType { None, White, Black };

    Piece() = default;
    Piece( const PieceType &a_pieceType ) :
        pieceType( a_pieceType ),
        isKing(false)
    {}

    PieceType pieceType;
    bool isKing;

    /// Return the opponents piece type.
    static PieceType GetOpponentPieceType( PieceType pieceType )
    {
        return pieceType == PieceType::White ? PieceType::Black : PieceType::White;
    }

    bool operator== (const Piece& rhs) const
    {
        return pieceType==rhs.pieceType && isKing==rhs.isKing;
    }

    bool operator!= (const Piece& rhs) const
    {
        return !(*this==rhs);
    }
};

}
