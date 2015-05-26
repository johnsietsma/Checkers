#pragma once

#include "Move.h"

namespace checkers {

// fwd decls
class CheckersBoard;

class AIPlayer
{
public:
	Move ChooseBestMove( const CheckersBoard& board ) const;
};

}
