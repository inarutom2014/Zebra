/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-25 16:01:48
**/

#ifndef _RNG_HPP_
#define _RNG_HPP_

#include <random>
#include <ctime>

namespace Zebra {

class Rng
{
	public:
		Rng():generator_(time(0)), distribution_(0, 1) { }

		double Get1() {
			return distribution_(generator_);
		}

		Point2 Get2() {
			return Point2(distribution_(generator_), distribution_(generator_));
		}

	private:
		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;
};

} // namespace Zebra

#endif /* _RNG_HPP_ */