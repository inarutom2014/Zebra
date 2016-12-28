/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-11-30 18:10:33
**/

#include <dirent.h>
#include <fstream>
#include <sstream>

#include "multi_svm.hpp"

namespace Elephant {

MultiSVM::MultiSVM(const char *train, const char *test, float rate, float reg, size_t iter,
                   size_t batch)
:train_dir_(train), test_dir_(test), rate_(rate), reg_(reg), max_iter_(iter), batch_(batch) { }

std::string MultiSVM::ToString() const
{
	std::ostringstream os;
	// os << "MultiSVM info:\n";
	// os << "train data:   \033[31m" << x_.ToString() << "\033[0m";
	// os << "train label:  \033[32m" << y_.size() << std::endl << "\033[0m";
	// os << "weight: \033[34m" << w_.ToString() << "\033[0m";
	os << rate_ << " " << reg_ << " " << max_iter_ << " " << batch_ << std::endl;
	os << "loss: \033[31m" << loss_ << "\033[0m\n";
	// os << "test data:    \033[31m" << x_test_.ToString() << "\033[0m";
	// os << "test label:   \033[32m" << y_test_.size() << std::endl << "\033[0m";
	os << "error rate: \033[31m" <<  err_rate_ << "\033[0m %\n";

	return std::move(os.str());
}

void LoadModel(const char *dir_name, Matrix<uint8_t> &x, Vector<uint8_t> &y, size_t total)
{
	DIR *dir = opendir(dir_name);
	assert(dir);
	struct dirent *dirp = nullptr;
	std::string file(dir_name);
	file += '/';
	for (size_t c = 0; (dirp = readdir(dir)) && c != total; ++c) {
		if (dirp->d_name[0] == '.') { --c; continue; }
		std::ifstream in(file + dirp->d_name);
		assert(in.is_open());
		y.Push(dirp->d_name[0] - '0');
		Vector<uint8_t> tmp(1025);
		for (size_t i = 0; !in.eof(); ) {
			std::string str;
			in >> str;
			for (auto e : str)
				tmp[i++] = e - '0';
		}
		tmp[1024] = 1;
		x.Push(tmp);
		in.close();
	}
	closedir(dir);
	assert(x.Shape().first == y.size());
}

float MultiSVM::ComputerLoss(const Vector<size_t> &index)
{
	float loss = 0;
	auto pair = w_.Shape();
	dw_ = Matrix<float>(pair);
	for (size_t i = 0; i != batch_; ++i) {
		size_t idx = index[i];
		auto scores = w_.Dot(x_[idx]);
		auto correct = scores[y_[idx]];
		scores -= (correct + 1);
		size_t sum = 0;
		for (size_t j = 0; j != pair.first; ++j) {
			if (scores[j] > 0) {
				if (y_[idx] != j) {
					loss += scores[j];
					dw_[j] += x_[idx];
					++sum;
				}
			}
		}
		dw_[y_[idx]] -= x_[idx] * sum;
	}
	loss /= batch_;
	dw_  /= batch_;
	loss += 0.5 * reg_ * (w_ * w_).Sum();
	return loss;
}

void MultiSVM::Train()
{
	LoadModel(train_dir_, x_, y_, 1000);
	auto pair = x_.Shape();
	assert(pair.first == y_.size());
	w_ = Matrix<float>::Randomize(10, pair.second, 0.1);

	for (size_t i = 0; i != max_iter_; ++i) {
		auto index = Vector<size_t>::RandomIndex(batch_, pair.first);
		loss_ = ComputerLoss(index);
		// std::cout << loss_ << std::endl;
		w_ -= dw_ * rate_;
	}
}

void MultiSVM::Predict()
{
	LoadModel(test_dir_, x_test_, y_test_, 1000);
	auto shape = x_test_.Shape();
	int error = 0;
	for (size_t i = 0; i != shape.first; ++i) {
		auto y_predict = w_.Dot(x_test_[i]);
		auto pred = y_predict.ArgMax();
		if (pred != y_test_[i])
			++error;
	}
	err_rate_ = (((float)error / shape.first) * 100);
}

} // namespace Elephant
