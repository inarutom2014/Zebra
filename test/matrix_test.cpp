/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-03 10:38:18
**/

#include "unit.hpp"
#include "../elephant/include/matrix.hpp"

using namespace Elephant;

TEST(MatrixT)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(4, 3, 5);
	m1 = m1.T();
	ASSERT_TRUE(m1 == m2);
	ASSERT_FALSE(m1 != m2);
}

TEST(MatrixDot1)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(4, 3, 5);
	Matrix<size_t> m3(3, 3, 100);
	auto m4 = m1.Dot(m2);
	ASSERT_TRUE(m3 == m4);
}

TEST(MatrixDot2)
{
	Matrix<size_t> m1(3, 4, 5);
	Vector<size_t> v1(4, 5);
	Vector<size_t> v2(3, 100);
	auto v3 = m1 * v1;
	ASSERT_TRUE(v3 == v2);
}

TEST(MatrixPlus1)
{
	Matrix<size_t> m1(3, 4, 4);
	Matrix<size_t> m2(3, 4, 1);
	Matrix<size_t> m3(3, 4, 5);
	auto m4 = m1 + m2;
	ASSERT_TRUE(m3 == m4);
}

TEST(MatrixPlus2)
{
	Matrix<size_t> m1(3, 4, 4);
	Matrix<size_t> m2(3, 4, 1);
	Matrix<size_t> m3(3, 4, 5);
	m1 += m2;
	ASSERT_TRUE(m1 == m3);
}

TEST(MatrixPlus3)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 6);
	auto m3 = m1 + 1;
	ASSERT_TRUE(m2 == m3);
}

TEST(MatrixPlus4)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 6);
	m1 += 1;
	ASSERT_TRUE(m1 == m2);
}

TEST(MatrixMultiply1)
{
	Matrix<size_t> m1(2, 2, 5);
	Matrix<size_t> m2(2, 2, 5);
	Matrix<size_t> m3(2, 2, 25);
	m1 *= m2;
	ASSERT_TRUE(m1 == m3);
}

TEST(MatrixMultiply2)
{
	Matrix<size_t> m1(2, 2, 5);
	Matrix<size_t> m2(2, 2, 5);
	Matrix<size_t> m3(2, 2, 25);
	auto m4 = m1 * m2;
	ASSERT_TRUE(m3 == m4);
}

TEST(MatrixMultiply3)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 25);
	auto m3 = m1 * 5;
	ASSERT_TRUE(m2 == m3);
}

TEST(MatrixMultiply4)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 25);
	m1 *= 5;
	ASSERT_TRUE(m1 == m2);
}

TEST(MatrixDivide1)
{
	Matrix<double> m1(2, 2, 3);
	Matrix<double> m2(2, 2, 1);
	Vector<double> v1(2, 3);
	auto m3 = m1 / v1;
	ASSERT_TRUE(m2 == m3);
}

TEST(MatrixDivide2)
{
	Matrix<double> m1(2, 2, 3);
	Matrix<double> m2(2, 2, 1);
	Vector<double> v1(2, 3);
	m1 /= v1;
	ASSERT_TRUE(m1 == m2);
}

TEST(MatrixDivide3)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 1);
	auto m3 = m1 / 5;
	ASSERT_TRUE(m2 == m3);
}

TEST(MatrixDivide4)
{
	Matrix<size_t> m1(3, 4, 5);
	Matrix<size_t> m2(3, 4, 1);
	m1 /= 5;
	ASSERT_TRUE(m1 == m2);
}

TEST(MatrixMax)
{
	Matrix<size_t> m1(3, 4, 5);
	m1[2][3] = 6;
	ASSERT_EQ(size_t(6), m1.Max());
}

TEST(MatrixSum)
{
	Matrix<size_t> m1(3, 4, 5);
	ASSERT_EQ(size_t(60), m1.Sum());
	Matrix<double> m2(4, 4, 2.5);
	Matrix<double> m3(4, 4, 2.5);
	ASSERT_TRUE(m2.Sum(0) == m3.Sum(1));
}

int main()
{
	return RUN_ALL_TESTS();
}
