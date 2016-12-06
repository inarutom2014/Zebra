/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-04 15:16:36
**/

#include <iostream>

#include "unit.hpp"
#include "../src/svm/svm.hpp"

using namespace Bunny;

TEST(SVMLoadModelTest)
{
	SVM svm("../data/test", "../data/train", 1e-3, 1e-5, 10);
	std::cout << svm.ToString();
}

TEST(SVMTrainTest)
{
	SVM svm("../data/test", "../data/train", 1e-3, 1e-5, 10);
	svm.Train();
	std::cout << svm.ToString();
}

int main()
{
	return RUN_ALL_TESTS();
}