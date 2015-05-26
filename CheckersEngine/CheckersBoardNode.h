#pragma once

#include <assert.h>
#include <cstdlib>

#include "CheckersBoard.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <random>
#include <vector>

namespace checkers {


class CheckersBoardNode
{
public:
	CheckersBoardNode(CheckersBoardNode* parent, const CheckersBoard& board, Move move, CheckersBoard::WinType winType) :
		m_parent(parent),
		m_board(board),
		m_move(move),
		m_winType(winType),
		m_wins(0)
	{
		m_board.GetMoves(m_moves);
	}

	const CheckersBoardNode* GetParent() const { return m_parent;  }

	Move GetMove() const { return m_move;  }

	bool IsWin() const
	{
		return m_board.IsFinished() && m_board.GetWinner() == m_winType;
	}

	int GetWins() const { return m_wins;  }

	void PropogateWin()
	{
		if (!IsWin()) return;
		CheckersBoardNode* parentNode = m_parent;
		while (parentNode != nullptr) {
			parentNode->m_wins++;
			parentNode = parentNode->m_parent;
		}
	}

	std::vector<CheckersBoardNode>& GetChildNodes()
	{
		if (m_children.empty()) CreateChildNodes();
		return m_children;
	}

	Move GetBestMove() const
	{
		auto bestChildNode = std::max_element(m_children.begin(), m_children.end(),
			[](const CheckersBoardNode& n1, const CheckersBoardNode& n2){ return n1.GetWins() < n2.GetWins(); });
		if (bestChildNode != m_children.end()) {
			return bestChildNode->GetMove();
		}
		return Move();
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
	void CreateChildNodes()
	{
		for (unsigned int i = 0; i < m_moves.size(); i++)
		{
			CheckersBoard childBoard(m_board);
			childBoard.DoMove(m_moves[i]);
			m_children.emplace_back( this, childBoard, m_moves[i], m_winType );
		}
	}

	CheckersBoardNode* m_parent;
	std::vector<CheckersBoardNode> m_children;

	CheckersBoard m_board;
	Move m_move;
	std::vector<Move> m_moves;

	CheckersBoard::WinType m_winType;
	int m_wins;
};
}
