/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-03 17:23:12
**/

#include "unit.hpp"
#include "../src/include/kernal.hpp"
#include "../src/include/matrix.hpp"

using namespace Bunny;

TEST(LinearKernal)
{
	Kernal kernal;
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(4, 3, 5);
	Matrix<size_t> m3(3, 3, 100);
	auto m4 = kernal(m1, m2);
	EXPECT_TRUE(m3 == m4);
}

int main()
{
	return RUN_ALL_TESTS();
}
