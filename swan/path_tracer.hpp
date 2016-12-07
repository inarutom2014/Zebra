/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 23:35:11
**/

#ifndef _PATH_TRACER_HPP_
#define _PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "isect.hpp"
#include "scene.hpp"
#include "integrator.hpp"
#include "parser.hpp"

namespace Swan {

class PathTracer : public Integrator
{
	public:
		PathTracer(int samples, const Point2<int> &pixel_bound, int max_depth)
		:Integrator(samples, pixel_bound), max_depth_(max_depth) { }

		Spectrum Li(const Ray &ray, ) const override {
			Spectrum L, weight(1);
			for (int bounce = 0; ; ++bounce) {
				Isect isect;
				if (!scene_.Intersect(ray, isect) || bounce > max_depth_) break;



				if (bounce > 3) {
					double p = std::max(weight.x_, std::max(weight.y_, weight.z_));
					if (distribution(generator) < p) break;
					weight /= 1 - p;
				}
			}
			return L;
		}

	private:
		int max_depth_;
};

} // namespace Swan

#endif /* _PATH_TRACER_HPP_ */