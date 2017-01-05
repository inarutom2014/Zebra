/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-27 23:21:50
**/

#include <iostream>

#include "unit.hpp"
#include "../elephant/include/batch.hpp"
#include "../elephant/nn/nn.hpp"

using namespace Elephant;

double rate;
double reg;
size_t iter;
size_t batch;

TEST(NeuralNetwork)
{
	std::vector<size_t> layers = {100, 10};
	NeuralNetwork nn(layers, Activation(Activation::ReLU));
	Batch train_data("../data/training");
	train_data.Load();
	nn.Train(rate, iter, batch, reg, train_data);
	Batch test_data("../data/test");
	test_data.Load();
	nn.Predict(test_data.X(), test_data.Y());
}

int main(int argc, char **argv)
{
	if (argc < 5) return 0;
	rate = atof(argv[1]);
	iter = atoi(argv[2]);
	batch = atoi(argv[3]);
	reg = atof(argv[4]);
	return RUN_ALL_TESTS();
}
