/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-06 09:47:22
**/

#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <vector>

#include "object.hpp"
#include "light.hpp"
#include "isect.hpp"
#include "ray.hpp"

namespace Zebra {

class Scene
{
	public:
		Scene() { }

		Scene(const std::vector<Object *> &objects, const std::vector<Light *> &lights)
		:objects_(objects), lights_(lights) { }

		bool Intersect(const Ray &ray, Isect &isect) const {
			bool flag = false;
			for (auto e : objects_)
				if (e->Intersect(ray, isect))
					flag = true;
			return flag;
		}

		bool IntersectP(const Ray &ray, double distance) const {
			for (auto e : objects_)
				if (e->IntersectP(ray, distance))
					return true;
			return false;
		}

		const std::vector<Object *>& Objects() const { return objects_; }
		const std::vector<Light *>& Lights() const { return lights_; }

	private:
		const std::vector<Object *> objects_;
		const std::vector<Light *>  lights_;
};

} // namespace Zebra

#endif /* _SCENE_HPP_ */