#pragma once

#include <assert.h>
#include <cstdlib>

#include "CheckersBoard.h"

#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

namespace checkers {

class CheckersBoardNode
{
public:
	CheckersBoardNode(const CheckersBoard& a_board) :
		m_boardState(a_board)
	{
		m_boardState.GetMoves(m_moves);
	}

	bool IsLeafNode() const
	{
		return m_boardState.IsFinished();
	}

	Move GetRandomMove() const
	{
		assert(!m_moves.empty());
		std::random_device rng;
		std::default_random_engine urng(rng());
		std::uniform_int_distribution<int> dist(0, m_moves.size()-1);
		return m_moves[dist(urng)];
	}

private:
	CheckersBoard m_boardState;
	std::vector<Move> m_moves;
};
}
