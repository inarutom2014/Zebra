/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-18 18:13:18
**/

#ifndef _PRIMITIVE_HPP_
#define _PRIMITIVE_HPP_

#include <cassert>

#include "object.hpp"
#include "light.hpp"
#include "bsdf.hpp"
#include "intersection.hpp"

namespace Zebra {

class Primitive
{
	public:
		Primitive(const Object *object, const BSDF *bsdf, const AreaLight *light = nullptr)
		:object_(object), bsdf_(bsdf), light_(light) { }

		bool Intersect(Ray &r, Intersection &i) const {
			if (!object_->Intersect(r, i)) return false;
			i.primitive_ = this;
			i.bsdf_      = bsdf_;
			return true;
		}

		bool IntersectP(const Ray &r) const {
			return object_->IntersectP(r);
		}

		const AreaLight* GetAreaLight() const { return light_; }

		~Primitive() {
			if (!light_)
				delete object_;
			delete bsdf_;
		}

	// private:
		const Object    *object_;
		const BSDF      *bsdf_;
		const AreaLight *light_;
};

} // namespace Zebra

#endif /* _PRIMITIVE_HPP_ */