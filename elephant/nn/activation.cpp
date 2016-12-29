/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-29 12:31:54
**/

#include "activation.hpp"

namespace Elephant {

Matrix<double> Sigmoid_Forward(const Matrix<double> &m)
{
	return std::move(1.0 / ((-m).Exp() + 1.0));
}

Matrix<double> Sigmoid_Backward(const Matrix<double> &m)
{
	auto tmp = m.Exp();
	return std::move(tmp * (1.0 - tmp));
}

Matrix<double> ReLU_Forward(const Matrix<double> &m)
{
	auto shape = m.Shape();
	Matrix<double> res(shape);
	for (size_t i = 0; i != shape.first; ++i)
		for (size_t j = 0; j != shape.second; ++j) {
			auto tmp = m[i][j];
			res[i][j] = tmp < 0 ? 0 : tmp;
		}
	return res;
}

Matrix<double> ReLU_Backward(const Matrix<double> &m)
{
	return std::move(ReLU_Forward(m));
}

} // namespace Elephant
