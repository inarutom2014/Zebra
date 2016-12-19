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
#include "vertex.hpp"

namespace Zebra {

class BSDF;
class Primitive;

class Intersection : public Vertex
{
	public:
		Intersection() { }

		Intersection(const Point &position, const Vector &normal)
		:Vertex(position, normal), primitive_(nullptr), bsdf_(nullptr) { }

		const Primitive *primitive_;
		const BSDF      *bsdf_;
};

} // namespace Zebra

#endif /* _INTERSECTION_HPP_ */