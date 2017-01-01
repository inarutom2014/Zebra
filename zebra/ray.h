/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-31 23:36:40
**/

#ifndef _RAY_H_
#define _RAY_H_

#include "vector.h"

namespace Zebra {

class Ray
{
	public:
		Ray(const Point &o, const Vector &d, double m = 1e10):o_(o), d_(d), m_(m) { }

		Point  o_;
		Vector d_;
		double m_;
};

} // namespace Zebra

#endif /* _RAY_H_ */