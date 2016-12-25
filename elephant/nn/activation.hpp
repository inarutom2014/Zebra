/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-25 21:52:56
**/

#ifndef _ACTIVATION_HPP_
#define _ACTIVATION_HPP_

namespace Elephant {

void Sigmoid_Forward(Matrix<double> &m)
{

}

void Sigmoid_Backward(Matrix<double> &m)
{

}

void ReLU_Forward(Matrix<double> &m)
{

}

void ReLU_Backward(Matrix<double> &m)
{

}

class Activation
{
	public:
		enum ActivationType { Sigmoid, ReLU } ;

		Activation():forward_(nullptr), backward_(nullptr) { }

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

		void Forward(Matrix<double> &m) const { assert(forward_); forward_(m); }

		void Backward(Matrix<double> &m) const { assert(backward_); backward_(m); }

	private:
		void (*forward_)(Matrix<double> &m);
		void (*backward_)(Matrix<double> &m);
};

} // namespace Elephant

#endif /* _ACTIVATION_HPP_ */