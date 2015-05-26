#include "AIPlayer.h"

#include "CheckersBoard.h"
#include "CheckersBoardNode.h"

#include <queue>
#include <vector>

using namespace checkers;

Move AIPlayer::ChooseBestMove(const CheckersBoard& board) const
{
	CheckersBoardNode topNode(nullptr, board, Move(), CheckersBoard::GetWinTypeFromSideType(board.GetCurrentSide()));

	std::queue<CheckersBoardNode*> nodeStack;
	nodeStack.push(&topNode);

	while (!nodeStack.empty())
	{
		CheckersBoardNode* currentNode = nodeStack.front();
		nodeStack.pop();

		auto childNodes = currentNode->GetChildNodes();

		for (auto childNode : childNodes)
		{
			nodeStack.push(&childNode);
			childNode.PropogateWin();
		}
	}

	return topNode.GetBestMove();
}
