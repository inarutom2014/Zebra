/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-11-30 18:10:16
**/

#ifndef _MULTI_SVM_HPP_
#define _MULTI_SVM_HPP_

#include <string>

#include "../include/vector.hpp"
#include "../include/matrix.hpp"

namespace Bunny {

class MultiSVM
{
	public:
		MultiSVM(const char *train, const char *test, float rate, float rag, size_t iter,
			size_t batch);

		void Train();

		void Predict();

		std::string ToString() const;

	private:
		float ComputerLoss(const Vector<size_t> &index);

		const char *train_dir_;
		const char *test_dir_;

		float           rate_;
		float           reg_;
		size_t          max_iter_;
		size_t          batch_;

		Matrix<uint8_t> x_;
		Vector<uint8_t> y_;

		Matrix<uint8_t> x_test_;
		Vector<uint8_t> y_test_;

		Vector<float>  loss_;

		Matrix<float>  w_;
		Matrix<float>  dw_;

		float          err_rate_ = 0.0;
};

} // namespace Bunny

#endif /* _MULTI_SVM_HPP_ */