/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-09 22:49:39
**/

#ifndef _NEURAL_NETWORK_HPP_
#define _NEURAL_NETWORK_HPP_

#include <string>

#include "../include/vector.hpp"
#include "../include/matrix.hpp"
#include "activation.hpp"

namespace Elephant {

class NeuralNetwork
{
	public:
		NeuralNetwork(const std::vector<size_t> &layers);

		void Train(double rate, size_t max_iter, size_t batch, const Matrix<uint8_t> &x,
			const Vector<uint8_t> &y);

		double ComputeLoss(const Vector<size_t> &index);

		std::string ToString() const;

	private:
		std::vector<size_t>         layers_;
		std::vector<Matrix<double>> w_;
		std::vector<Vector<double>> b_;

		std::vector<Matrix<double>> dw_;
		std::vector<Vector<double>> db_;
};

} // namespace Elephant

#endif /* _NEURAL_NETWORK_HPP_ */