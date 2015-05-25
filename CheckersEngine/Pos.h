#pragma once

#include <cstdlib>

namespace checkers {

struct Pos
{
    int row;
    int column;

    int IsAdjacent(const Pos& p) const
    {
        return GetDistance(p)==1;
    }

    int GetDistance(const Pos& p) const
    {
        // Manhattan distance
        return std::abs( p.row - row ) + std::abs( p.column - column );
    }

    bool IsDiagonal(const Pos& p) const
    {
        return std::abs( p.row - row ) == std::abs( p.column - column );
    }

    Pos Clamp1() const
    {
        int r = row/std::abs(row);
        int c = column/std::abs(column);
        return { r, c };
    }

    bool operator== (const Pos& rhs) const
    {
        return row==rhs.row && column==rhs.column;
    }

    bool operator!= (const Pos& rhs) const
    {
        return !(*this==rhs);
    }

    Pos operator- ( const Pos& p ) const
    {
        return { row - p.row, column - p.column };
    }

    Pos operator+ ( const Pos& p ) const
    {
        return { row + p.row, column + p.column };
    }

};


}
