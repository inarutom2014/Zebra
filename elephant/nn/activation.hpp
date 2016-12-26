/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-25 21:52:56
**/

#ifndef _ACTIVATION_HPP_
#define _ACTIVATION_HPP_

#include <cmath>

#include "../include/matrix.hpp"

namespace Elephant {

Matrix<double> Sigmoid_Forward(const Matrix<double> &m)
{
	return std::move(1.0 / (1.0 + (-m).Exp()));
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
			if (tmp < 0)
				res[i][j] = 0;
			else
				res[i][j] = tmp;
		}
	return std::move(res);
}

Matrix<double> ReLU_Backward(const Matrix<double> &m)
{
	auto shape = m.Shape();
	Matrix<double> res(shape);
	for (size_t i = 0; i != shape.first; ++i)
		for (size_t j = 0; j != shape.second; ++j) {
			auto tmp = m[i][j];
			if (tmp < 0)
				res[i][j] = 0;
			else
				res[i][j] = tmp;
		}
	return std::move(res);
}

class Activation
{
	public:
		enum ActivationType { Sigmoid, ReLU } ;

		Activation(ActivationType type = Sigmoid) {
			switch (type) {
				case Sigmoid:
					forward_  = Sigmoid_Forward;
					backward_ = Sigmoid_Backward;
					break;
				case ReLU:
					forward_  = ReLU_Forward;
					backward_ = ReLU_Backward;
					break;
				default:
					exit(-1);
			}
		}

		Matrix<double> Forward(const Matrix<double> &m) const {
			assert(forward_);
			return forward_(m);
		}

		Matrix<double> Backward(const Matrix<double> &m) const {
			assert(backward_);
			return backward_(m);
		}

	private:
		Matrix<double> (*forward_)(const Matrix<double> &m);
		Matrix<double> (*backward_)(const Matrix<double> &m);
};

} // namespace Elephant

#endif /* _ACTIVATION_HPP_ */