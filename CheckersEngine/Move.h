#pragma once

#include "Pos.h"

namespace checkers {

/**
 * Defines a move from one Pos to another.
 */
struct Move
{
    Pos from;
    Pos to;

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
