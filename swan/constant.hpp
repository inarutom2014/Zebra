/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-05 14:39:56
**/

#ifndef _CONSTANT_HPP_
#define _CONSTANT_HPP_

#include <cmath>
#include <limits>

namespace Swan {

const double PI        = 3.141592653589793238;
const double INV_PI    = 1.0 / PI;
const double kInfinity = std::numeric_limits<double>::max();
const double kEpsilon  = 1e-4;
const double E         = 2.718281828459045;

inline double radian(double angle) {
	return angle * (PI / 180.0);
}

} // namespace Swan

#endif /* _CONSTANT_HPP_ */