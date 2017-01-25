/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-06 09:47:22
**/

#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <vector>

#include "ray.hpp"
#include "primitive.hpp"
#include "light.hpp"
#include "intersection.hpp"

namespace Zebra {

class Scene
{
	public:
		Scene() { }

		Scene(const std::vector<Primitive *> &primitives, const std::vector<Light *> &lights)
		:primitives_(primitives), lights_(lights) { }

		bool Intersect(Ray &ray, Intersection &isect) const {
			bool flag = false;
			for (auto e : primitives_)
				if (e->Intersect(ray, isect))
					flag = true;
			return flag;
		}

		bool IntersectP(const Ray &ray) const {
			for (auto e : primitives_)
				if (!e->GetAreaLight() && e->IntersectP(ray))
					return true;
			return false;
		}

		const std::vector<Primitive *>& Primitives() const { return primitives_; }
		const std::vector<Light *>& Lights() const { return lights_; }

	private:
		const std::vector<Primitive *> primitives_;
		const std::vector<Light *>     lights_;
};

} // namespace Zebra

#endif /* _SCENE_HPP_ */