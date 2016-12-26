/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-25 20:22:04
**/

#include <sstream>

#include "nn.hpp"

namespace Elephant {

NeuralNetwork::NeuralNetwork(const std::vector<size_t> &layers, const Activation &activation)
:layers_(layers), activation_(activation)
{
	w_ = std::vector<Matrix<double>>(layers_.size());
	b_ = std::vector<Vector<double>>(layers_.size());
	dw_ = std::vector<Matrix<double>>(layers_.size());
	db_ = std::vector<Vector<double>>(layers_.size());
}

void NeuralNetwork::Train(double rate, size_t max_iter, size_t batch, const Matrix<uint8_t> &x,
	const Vector<uint8_t> &y)
{
	assert(layers_.size());
	auto shape = x.Shape();
	size_t a = shape.second, b = layers_[0];
	for (size_t i = 0; ; ++i) {
		w_[i] = Matrix<double>::Randomize(a, b, 0.1);
		b_[i] = Vector<double>::Randomize(b, 0.1);
		if (i == (layers_.size() - 1))
			break;
		a = b;
		b = layers_[i + 1];
	}

	std::vector<double> loss(max_iter);
	for (size_t i = 0; i != max_iter; ++i) {
		auto index = Vector<size_t>::RandomIndex(batch, shape.first);
		Matrix<double> m(x, index);
		loss[i] = ComputeLoss(m);
		for (size_t j = 0; j != layers_.size(); ++j) {
			w_[j] -= dw_[j] * rate;
			b_[j] -= db_[j] * rate;
		}
	}
}

double NeuralNetwork::ComputeLoss(const Matrix<double> &x)
{
	std::vector<Matrix<double>> m(layers_.size());
	std::vector<Matrix<double>> n(layers_.size());
	n[0] = x;
	for (size_t i = 0; i != layers_.size(); ++i) {
		m[i] = n[i].Dot(w_[i]);
		m[i] += b_[i];
		n[i] = activation_.Forward(m[i]);
	}



}

} // namespace Elephant
