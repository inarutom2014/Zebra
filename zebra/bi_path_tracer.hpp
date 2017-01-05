/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-29 21:09:33
**/

#ifndef _BI_PATH_TRACER_HPP_
#define _BI_PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "integrator.hpp"

namespace Zebra {

class Vertex : public Isect
{
	public:

	private:
		const BSDF *bsdf_;
		Spectrum    weight_;
		double      pdf_;
};

class BiPathTracer : public Integrator
{
	public:
		BiPathTracer(int iterations, const Scene &scene)
		:Integrator(iterations, scene) { }

		std::string Render() {
			int path_count = camera_.resolution_.x_ * camera_.resolution_.y_;
			auto lights = scene_.Lights();
			for (int i = 0; i != iterations_; ++i) {
				for (int j = 0; j != path_count; ++j) {
					double pdf_pos, pdf_dir;
					Ray light_ray;
					Spectrum l = lights[0]->SampleLe(light_ray, rng_.Get2D(), pdf_pos, pdf_dir);
					l *= 1.0 / (pdf_pos * pdf_dir);

					Intersection isect;
					if (!scene_.Intersect(light_ray, isect)) break;

				}
			}
		}

	private:

};

} // namespace Zebra

#endif /* _BI_PATH_TRACER_HPP_ */