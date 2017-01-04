/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 14:35:51
**/

#ifndef _INTERSECTION_HPP_
#define _INTERSECTION_HPP_

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"

namespace Zebra {

class BSDF;
class Light;
class Camera;
class Primitive;

class Isect
{
	public:
		Isect() { }

		Isect(const Point &position, const Vector &normal):position_(position), normal_(normal) { }

		Point  position_;
		Vector normal_;
};

class Intersection : public Isect
{
	public:
		Intersection() { }

		Intersection(const Point &position, const Vector &normal)
		:Isect(position, normal), primitive_(nullptr), bsdf_(nullptr) { }

		const Primitive *primitive_;
		const BSDF      *bsdf_;
};

} // namespace Zebra

#endif /* _INTERSECTION_HPP_ */