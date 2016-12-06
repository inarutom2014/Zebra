/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-11-30 18:10:33
**/

#include <dirent.h>
#include <fstream>
#include <sstream>

#include "svm.hpp"

namespace Bunny {

SVM::SVM(const char *train, const char *test, float rate, float reg, size_t batch)
:train_dir_(train), test_dir_(test), rate_(rate), reg_(reg), batch_(batch), b_(0) { }

std::string SVM::ToString() const
{
	std::ostringstream os;
	os << "SVM info:\n";
	os << "data:   \033[31m" << x_.ToString() << "\033[0m";
	os << "label:  \033[32m" << y_.size() << std::endl << "\033[0m";
	os << "weight: \033[34m" << w_.ToString() << "\033[0m";
	if (loss_.size()) {
		for (size_t i = 0; i != loss_.size(); ++i)
			os << loss_[i] << " ";
	}
	return std::move(os.str());
}

void LoadModel(const char *dir_name, Matrix<uint8_t> &x, Vector<uint8_t> &y)
{
	DIR *dir = opendir(dir_name);
	assert(dir);
	struct dirent *dirp = nullptr;
	std::string file(dir_name);
	file += '/';
	// size_t c = 0;
	for (size_t c = 0; (dirp = readdir(dir)) && c != 10; ++c) {
		if (dirp->d_name[0] == '.') { --c; continue; }
		// std::cout << dirp->d_name << std::endl;
		std::ifstream in(file + dirp->d_name);
		assert(in.is_open());
		y.Push(dirp->d_name[0] - '0');
		Vector<uint8_t> tmp(1024);
		for (size_t i = 0; !in.eof(); ) {
			std::string str;
			in >> str;
			// std::cout << str << std::endl;
			for (auto e : str)
				tmp.Assign(i++, e - '0');
		}
		x.Push(tmp);
		in.close();
	}
	// std::cout << c << std::endl;
	closedir(dir);
	assert(x.Shape().first == y.size());
}

float SVM::ComputerLoss(const Vector<size_t> &index)
{
	float loss = 0;
	auto pair = w_.Shape();
	dw_ = Matrix<float>(pair);
	for (size_t i = 0; i != batch_; ++i) {
		size_t idx = index[i];
		auto scores = w_.Dot(x_[idx]);
		auto correct = scores[y_[idx]];
		scores -= (correct + 1);
		for (size_t j = 0; j != pair.first; ++j) {
			if (y_[idx] != j) {
				if (scores[j] > 0) {
					loss += scores[j];
					dw_[j] += x_[idx];
				}
			} else {
				size_t sum = 0;
				for (size_t k = 0; k != pair.first; ++k)
					if (scores[k] > 0 && k != j)
						++sum;
				dw_[j] -= x_[idx] * sum;
			}
		}
	}
	std::cout << "fuck\n";
	loss /= pair.first;
	dw_  /= pair.first;
	loss += 0.5 * reg_ * (w_ * w_).Sum();
	dw_ += reg_;
	return loss;
}

void SVM::Train()
{
	LoadModel(train_dir_, x_, y_);
	auto pair = x_.Shape();
	assert(pair.first == y_.size());
	w_    = Matrix<float>(10, pair.second);
	w_.Randomize();

	size_t max_iter = 1;
	loss_ = Vector<float>(max_iter);

	for (size_t i = 0; i != max_iter; ++i) {
		Vector<size_t> index(batch_);
		index.Randomize(batch_, pair.first);
		loss_.Assign(i, ComputerLoss(index));
		w_ -= dw_ * rate_;
	}
}

void SVM::Predict()
{
	LoadModel(test_dir_, x_test_, y_test_);
	auto shape = x_test_.Shape();
	int error = 0;
	for (size_t i = 0; i != shape.first; ++i) {
		auto y_predict = w_.Dot(x_test_[i]);
		if (y_predict.ArgMax() != y_test_[i])
			++error;
	}
	std::cerr << "error rate: \033[31m" << (float)error / shape.first << "\033[0m\n";
}

} // namespace Bunny
