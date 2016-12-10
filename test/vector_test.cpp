/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-03 09:01:48
**/

#include <iostream>

#include "unit.hpp"
#include "../elephant/include/vector.hpp"

using namespace Elephant;

TEST(VectorDot)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 4);
	Vector<size_t> v2 = Vector<size_t>::Arange(2, 5);
	ASSERT_EQ(size_t(20), v1.Dot(v2));
}

TEST(VectorPlus1)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	Vector<size_t> v2 = Vector<size_t>::Arange(2, 4);
	auto v3 = v1 + v2;
	v1 += v2;
	ASSERT_TRUE(v1 == v3);
	ASSERT_FALSE(v1 != v3);
}

TEST(VectorPlus2)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	auto v2 = v1 + 2;
	v1 += 2;
	ASSERT_TRUE(v1 == v2);
}

TEST(VectorMinus1)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	Vector<size_t> v2 = Vector<size_t>::Arange(2, 4);
	auto v3 = v1 - v2;
	v1 -= v2;
	ASSERT_TRUE(v1 == v3);
}

TEST(VectorMinus2)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	auto v2 = v1 - 2;
	v1 -= 2;
	ASSERT_TRUE(v1 == v2);
}

// TEST(VectorMultiply1)
// {
// 	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
// 	Vector<size_t> v2 = Vector<size_t>::Arange(2, 4);
// 	auto v3 = v1 * v2;
// 	v1 *= v2;
// 	ASSERT_TRUE(v1 == v3);
// }

TEST(VectorMultiply1)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	auto v2 = v1 * 2;
	v1 *= 2;
	ASSERT_TRUE(v1 == v2);
}

TEST(VectorDivide)
{
	Vector<size_t> v1 = Vector<size_t>::Arange(1, 3);
	auto v2 = v1 / 2;
	v1 /= 2;
	ASSERT_TRUE(v1 == v2);
}

TEST(VectorRandomize)
{
	Vector<size_t> v1(10);
	v1.Randomize(10, 10);
	for (auto i = v1.begin(); i != v1.end(); ++i)
		std::cout << *i << " ";
	std::cout << std::endl;
}

int main()
{
	return RUN_ALL_TESTS();
}
