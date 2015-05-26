#include "AIPlayer.h"

#include "CheckersBoard.h"
#include "CheckersBoardNode.h"

using namespace checkers;

Move AIPlayer::ChooseBestMove(const CheckersBoard& board) const
{
	CheckersBoardNode node(board);
	return node.GetRandomMove();
}
