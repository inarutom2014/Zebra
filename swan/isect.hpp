/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-05 14:35:51
**/

#ifndef _ISECT_HPP_
#define _ISECT_HPP_

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace Swan {

class Isect
{
	public:
		Isect():distance_(kInfinity) { }

		double Miss() const { return distance_ == kInfinity; }

		double Distance() const { return distance_; }

		Vector Normal() const { return normal_; }

		const BSDF* Bsdf() const { return bsdf_; }

		void Update(double distance,
                const Point &position,
                const Vector &normal,
                const BSDF *bsdf) {
      distance_ = distance;
      position_ = position;
      normal_   = normal;
      bsdf_     = bsdf;
		}

	private:
		double      distance_;
		Point       position_;
		Vector      normal_;
		const BSDF *bsdf_;
};

} // namespace Swan

#endif /* _ISECT_HPP_ */