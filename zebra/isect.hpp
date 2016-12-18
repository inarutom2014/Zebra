/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 14:35:51
**/

#ifndef _ISECT_HPP_
#define _ISECT_HPP_

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "primitive.hpp"

namespace Zebra {

class Isect
{
	public:
		Isect() { }

		void Isect(const Point &position, const Vector &normal)
		:position_(position), normal_(normal) { }

	private:
		Point            position_;
		Vector           normal_;
		const Primitive *primitive_;
		const BSDF      *bsdf_;
};

} // namespace Zebra

#endif /* _ISECT_HPP_ */