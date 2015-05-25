#pragma once

#include "Pos.h"

#include <assert.h>

namespace checkers {

/**
 * Defines a move from one Pos to another.
 */
struct Move
{
    Pos from;
    Pos to;

	bool IsAdjacentMove() const { return from.GetDistance(to) == 2 && from.IsDiagonal(to); }
	bool IsJumpMove() const { return from.GetDistance(to) == 4 && from.IsDiagonal(to); }

	/// The jump pos is the Pos of the piece that is being jumped.
	Pos GetJumpPos() const { assert(IsJumpMove()); return from + (to - from).Clamp1(); }

    bool operator== (const Move& rhs) const
    {
        return from == rhs.from && to == rhs.to;
    }

    bool operator!= (const Move& rhs) const
    {
        return !(*this == rhs);
    }
};

}
