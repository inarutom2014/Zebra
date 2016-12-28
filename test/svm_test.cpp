/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-04 15:16:36
**/

#include <iostream>

#include "unit.hpp"
#include "../elephant/svm/multi_svm.hpp"

using namespace Elephant;

float rate;
float reg;
size_t iter;
size_t batch ;

TEST(SVMTrainTest)
{
	MultiSVM svm("../data/test", "../data/training", rate, reg, iter, batch);
	svm.Train();
	svm.Predict();
	std::cout << svm.ToString();
}

int main(int argc, char **argv)
{
	if (argc < 5) return 0;
	rate = (float)atof(argv[1]);
	reg = (float)atof(argv[2]);
	iter = atoi(argv[3]);
	batch = atoi(argv[4]);
	return RUN_ALL_TESTS();
}
