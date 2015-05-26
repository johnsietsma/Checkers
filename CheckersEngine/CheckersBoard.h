#pragma once

#include <assert.h>
#include <tuple>
#include <vector>

#include "Move.h"
#include "Piece.h"

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
	enum class WinType { White, Black, Draw };
	enum class SideType { White, Black };
    enum class MoveError {	None, IsOutOfBounds, IsOccupied, IsNotDiagonal, IsNotAdjacent, NoJumpPiece, // 5
							TooFar, NoPieceToMove, WrongSide, IsBackwards, MustJump
                         };

    const static int NumberOfSquares = 64;
    const static int NumberOfColumns = 8;
    const static int NumberOfRows = 8;

    /// Creates a new board with the default piece layout.
    CheckersBoard() : CheckersBoard( DefaultPieceLayout, SideType::White ) {}

    /// Create a board with a custom layout and starting side.
    CheckersBoard( const Piece::PieceType pieceTypes[NumberOfSquares], SideType currentSide );

	// Copy constructor
	CheckersBoard(const CheckersBoard& board);

    SideType GetCurrentSide() const { return m_currentSide;  }

	static WinType GetWinTypeFromSideType(SideType side)
	{
		return side == SideType::White ? WinType::White : WinType::Black;
	}

    /// Get the piece at the given position. Returns PieceType::None if ot of bounds.
    Piece GetPiece( const Pos &pos ) const;

    /// Sets a piece at a given location.
    void SetPiece( const Pos &pos, const Piece& piece );

	void RemovePiece( const Pos &pos );

	/// Get all the legal moves that the current side can make.
	void GetMoves(std::vector<Move> &moves) const;

	/// Get all the legal moves that the current side can make from the given Pos.
	void GetMoves(const Pos &pos, std::vector<Move> &moves) const;

    /// Get all the jump moves that the current size can make.
    void GetJumpMoves( std::vector<Move> &jumpMoves ) const;

    /// Get all the jump moves that the current side can make from the given Pos.
    void GetJumpMoves( const Pos &pos, std::vector<Move> &jumpMoves ) const;

    /// Returns whether the position is occupied. An out of bounds pos is considered occupied.
    bool IsOccupied( const Pos &pos ) const;

    // Returns whether a square is occupied by a particular PieceType.
    bool IsOccupied( const Pos &pos, Piece::PieceType pieceType ) const;

    /// Returns whether a position is out of bounds.
    bool IsOutOfBounds( const Pos &pos ) const;

    /// Returns whether a move can be made.
    bool CanMove( const Move &move ) const;

    /// Verifies a move and returns the error, can be MoveError::None.
    MoveError GetMoveError( const Move &move ) const;

    /// Performs the move
    void DoMove( const Move &move );

	bool IsFinished() const;

	WinType GetWinner() const;

private:
    /// Convert a Pos into an index into the board array.
    static int PosToIndex( const Pos &pos ) { return ( pos.row * NumberOfColumns ) + pos.column; }

	SideType GetCurrentOpponentSide() const
	{
		return GetCurrentSide() == SideType::White ? SideType::Black : SideType::White;
	}

	/// Get all the legal moves from startPos. Moves are made usingthe moveDeltas.
	// Legal moves are added to the moves vector.
	void GetMoves(const Pos &startPos, const Pos moveDeltas[4], std::vector<Move> &moves) const;

	// Returns a pair of bools that represent whether a side has pieces on the board. In White, Black order.
	std::tuple<bool, bool> GetSideHasPieces() const;

    /// Checks for all move errors, but doesn't return an error if there is a jump available.
    MoveError GetMoveError_DontForceJumps( const Move &move ) const;

    /// Returns true if the piece is on the far side of the board for its type.
    bool IsOnLastRow( const Piece::PieceType &pieceType, const Pos &pos ) const;

    /// The current side whose turn it is to move.
    SideType m_currentSide;

    /// The board array.
    Piece m_pieces[NumberOfSquares];

    // The default starting positions of all the pieces.
    static const Piece::PieceType DefaultPieceLayout[NumberOfSquares];

};

inline Piece CheckersBoard::GetPiece( const Pos &pos ) const
{
    if ( IsOutOfBounds( pos ) ) { return Piece::PieceType::None; }
    return m_pieces[PosToIndex( pos )];
}

inline void CheckersBoard::SetPiece( const Pos &pos, const Piece& piece )
{
    if ( IsOutOfBounds( pos ) ) { assert( false && "Piece out of bounds" ); return; }
    m_pieces[PosToIndex( pos )] = piece;
}

inline void CheckersBoard::RemovePiece(const Pos &pos)
{
	m_pieces[PosToIndex(pos)] = Piece(Piece::PieceType::None,false);
}

inline bool CheckersBoard::IsOccupied( const Pos &pos ) const
{
    return IsOccupied( pos, Piece::PieceType::Black ) || IsOccupied( pos, Piece::PieceType::White );
}

inline bool CheckersBoard::IsOccupied( const Pos &pos, Piece::PieceType pieceType ) const
{
    return IsOutOfBounds( pos ) || pieceType == m_pieces[PosToIndex( pos )].pieceType;
}

inline bool CheckersBoard::IsOutOfBounds( const Pos &pos ) const
{
    return  pos.row < 0 ||
            pos.row >= NumberOfRows ||
            pos.column < 0 ||
            pos.column >= NumberOfColumns;
}

inline bool CheckersBoard::CanMove( const Move &move ) const
{
    return GetMoveError( move ) == MoveError::None;
}

inline bool CheckersBoard::IsOnLastRow( const Piece::PieceType &pieceType, const Pos &pos ) const
{
    if ( pieceType == Piece::PieceType::White && pos.row == NumberOfRows - 1 ) {
        return true;
    }
    else if ( pieceType == Piece::PieceType::Black && pos.row == 0 ) {
        return true;
    }

    return false;
}

inline bool CheckersBoard::IsFinished() const
{
	auto sideHasPieces = GetSideHasPieces();
	return !std::get<0>(sideHasPieces) || !std::get<1>(sideHasPieces);
}

inline CheckersBoard::WinType CheckersBoard::GetWinner() const
{
	auto sideHasPieces = GetSideHasPieces();
	if (!std::get<0>(sideHasPieces) && !std::get<1>(sideHasPieces)) { return WinType::Draw; } // Can't happen in normal game
	if (std::get<0>(sideHasPieces) && !std::get<1>(sideHasPieces)) { return WinType::White; }
	if (!std::get<0>(sideHasPieces) && std::get<1>(sideHasPieces)) { return WinType::Black; }
	std::vector<Move> moves;
	GetMoves(moves);
	if (moves.empty()) return GetWinTypeFromSideType( GetCurrentOpponentSide() ); // Can't move means you lose

	/*
	TODO: Draw
	Their next move they would create the same position for the third time during the game.
	Neither player has advanced an uncrowned man towards the king-row during their own previous 40 moves.
	No pieces have been removed from the board during their own previous 40 moves.
	*/

	assert(false && "No winner");
	return WinType::Draw;
}

inline std::tuple<bool, bool> CheckersBoard::GetSideHasPieces() const
{
	bool hasWhite = false;
	bool hasBlack = false;

	for (auto piece : m_pieces)
	{
		if (piece.pieceType == Piece::PieceType::Black) { hasBlack = true; }
		if (piece.pieceType == Piece::PieceType::White) { hasWhite = true; }
	}

	return std::tuple<bool,bool>{ hasWhite, hasBlack };
}


}
