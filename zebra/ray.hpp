/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 18:40:19
**/

#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace Zebra {

class Ray
{
	public:
		Ray() { }

		Ray(const Point &origin, const Vector &direction, double max = kInfinity)
		:max_(max), origin_(origin), direction_(direction) { }

		double max_;
		Point  origin_;
		Vector direction_;
};

} // namespace Zebra

#endif /* _RAY_HPP_ */