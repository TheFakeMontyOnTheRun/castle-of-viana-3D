//
// Created by monty on 16/02/17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Vec2i.h"

using testing::Eq;

TEST( TestVec2i, equalityForEqualMembers ) {

	auto N = Knights::Vec2i{ 0, -1 };
	auto M = Knights::Vec2i{ 0, -1 };
	auto O = Knights::Vec2i{ 1, -1 };
	auto P = Knights::Vec2i{ 0, 1 };
	auto Q = Knights::Vec2i{ 1, 1 };
	ASSERT_EQ( N, M );
	ASSERT_EQ( M, N );
    ASSERT_EQ( N, N );

	ASSERT_NE( M, O );
	ASSERT_NE( M, P );
	ASSERT_NE( M, Q );

	ASSERT_NE( O, M );
	ASSERT_NE( P, M );
	ASSERT_NE( Q, M );

}