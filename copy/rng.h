/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Zebra
 *    > Created Time:  2017-01-01 09:05:26
**/

#ifndef _RNG_H_
#define _RNG_H_

#include <random>
#include <ctime>
#include <map>

#include "vector.h"

namespace Zebra {

class RNG
{
	public:
		RNG():generator_(time(0)), distribution_(0, 1) { }

		double Get1() {
			return distribution_(generator_);
		}

		Vector2 Get2() {
			return Vector2(distribution_(generator_), distribution_(generator_));
		}

	private:
		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;
};

} // namespace Zebra

#endif /* _RNG_H_ */