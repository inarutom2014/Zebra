/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-27 13:14:04
**/

#include <cmath>

#include "unit.hpp"
#include "../elephant/nn/activation.hpp"
#include "../elephant/include/matrix.hpp"

using namespace Elephant;

TEST(SigmoidForward)
{
	Matrix<double> m = Matrix<double>::Randomize(5, 5);
	Activation act;
	auto n = act.Forward(m);
	auto s = n.Shape();
	for (size_t i = 0; i != s.first; ++i)
		for (size_t j = 0; j != s.second; ++j)
			EXPECT_EQ(n[i][j], 1.0 / (1.0 + std::exp(-m[i][j])));
}

TEST(SigmoidBackward)
{
	Matrix<double> m = Matrix<double>::Randomize(5, 5);
	Activation act;
	auto n = act.Backward(m);
	auto s = n.Shape();
	for (size_t i = 0; i != s.first; ++i)
		for (size_t j = 0; j != s.second; ++j) {
			auto tmp = std::exp(m[i][j]);
			EXPECT_EQ(n[i][j], tmp * (1 - tmp));
		}
}

TEST(ReLUForward)
{
	Matrix<double> m(5, 5, -1);
	Activation act(Activation::ReLU);
	auto n = act.Forward(m);
	auto s = n.Shape();
	for (size_t i = 0; i != s.first; ++i)
		for (size_t j = 0; j != s.second; ++j)
			EXPECT_EQ(n[i][j], (m[i][j] > 0) ? m[i][j] : 0.0);
}

TEST(ReLUBackward)
{
	Matrix<double> m(5, 5, 2);
	Activation act(Activation::ReLU);
	auto n = act.Backward(m);
	auto s = n.Shape();
	for (size_t i = 0; i != s.first; ++i)
		for (size_t j = 0; j != s.second; ++j)
			EXPECT_EQ(n[i][j], (m[i][j] > 0) ? m[i][j] : 0.0);
}

int main()
{
	return RUN_ALL_TESTS();
}
