#include "Pos.h"

#include "gtest/gtest.h"

using namespace checkers;

TEST( pos_test, test_distance )
{
    EXPECT_EQ( 0, Pos( {5, 5} ).GetDistance( {5, 5} ) );
    EXPECT_EQ( 2, Pos( {7, 5} ).GetDistance( {5, 5} ) );
    EXPECT_EQ( 4, Pos( {5, 5} ).GetDistance( {7, 7} ) );
    EXPECT_EQ( 2, Pos( {2, 1} ).GetDistance( {3, 0} ) );
}

TEST( pos_test, test_diagonal )
{
    EXPECT_TRUE( Pos( {0, 0} ).IsDiagonal( {1, 1} ) );
    EXPECT_TRUE( Pos( { -1, -1} ).IsDiagonal( {1, 1} ) );
    EXPECT_FALSE( Pos( { -1, -1} ).IsDiagonal( {1, 2} ) );
}

TEST( pos_test, test_is_adjacent )
{
    EXPECT_FALSE( Pos( {1, 2} ).IsAdjacent( {4, 0} ) );
}


TEST( pos_test, test_addition )
{
    EXPECT_EQ( Pos( {1, 1} ) + Pos( {2, 3} ), Pos( {3, 4} ) );
}

TEST( pos_test, test_subtraction )
{
    EXPECT_EQ( Pos( {1, 1} ) - Pos( {2, 3} ), Pos( { -1, -2} ) );
}

TEST( pos_test, test_clamp1 )
{
    EXPECT_EQ( Pos( {1, 1} ), Pos( {2, 3} ).Clamp1() );
    EXPECT_EQ( Pos( {1, -1} ), Pos( {2, -3} ).Clamp1() );
}

TEST( pos_test, test_jump_calc )
{
    Pos from {0, 0};
    Pos to {2, 2};
    Pos jumpPos = from + ( to - from ).Clamp1();
    EXPECT_EQ( Pos( {1, 1} ), jumpPos );
}
