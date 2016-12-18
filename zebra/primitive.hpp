/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-18 18:13:18
**/

#ifndef _PRIMITIVE_HPP_
#define _PRIMITIVE_HPP_

#include "object.hpp"
#include "bsdf.hpp"
#include "light.hpp"

namespace Zebra {

class Primitive
{
	public:
		Primitive(const Object *object, const BSDF *bsdf, const AreaLight *light = nullptr)
		:object_(object), bsdf_(bsdf), light_(light) { }

		bool Intersect(const Ray &r, Isect &i) const {
			if (!object_->Intersect(r, i)) return false;
			i.primitive_ = this;
			i.bsdf_      = bsdf_;
			return true;
		}

		bool IntersectP(const Ray &r, double d) const {
			return object_.IntersectP(r, d);
		}

	private:
		const Object    *object_;
		const BSDF      *bsdf_;
		const AreaLight *light_;
};

} // namespace Zebra

#endif /* _PRIMITIVE_HPP_ */