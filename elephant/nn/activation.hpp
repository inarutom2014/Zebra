/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-25 21:52:56
**/

#ifndef _ACTIVATION_HPP_
#define _ACTIVATION_HPP_

#include <string>

#include "../include/matrix.hpp"

namespace Elephant {

Matrix<double> Sigmoid_Forward(const Matrix<double> &m);

Matrix<double> Sigmoid_Backward(const Matrix<double> &m);

Matrix<double> ReLU_Forward(const Matrix<double> &m);

Matrix<double> ReLU_Backward(const Matrix<double> &m);

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
			return forward_(m);
		}

		Matrix<double> Backward(const Matrix<double> &m) const {
			return backward_(m);
		}

		std::string ToString() const {
			if (forward_ == Sigmoid_Forward)
				return std::string("activation: \033[31m sigmoid\033[0m\n");
			else
				return std::string("activation: \033[31m ReLU\033[0m\n");
		}

	private:
		Matrix<double> (*forward_)(const Matrix<double> &m);
		Matrix<double> (*backward_)(const Matrix<double> &m);
};

} // namespace Elephant

#endif /* _ACTIVATION_HPP_ */