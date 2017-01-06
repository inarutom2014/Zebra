/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-29 21:09:33
**/

#ifndef _BI_PATH_TRACER_HPP_
#define _BI_PATH_TRACER_HPP_

#include <vector>

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "integrator.hpp"

namespace Zebra {

class Vertex : public Isect
{
	enum { Light, Surface, Camera } Type;

	public:
		Type        type_;
		Spectrum    weight_;
		union {
			const BSDF   *bsdf_;
			const Light  *light_;
			const Camera *camera_;
		};
};

class BiPathTracer : public Integrator
{
	public:
		BiPathTracer(int iterations, const Scene &scene)
		:Integrator(iterations, scene) { }

		std::string Render() {
			int path_count = camera_.resolution_.x_ * camera_.resolution_.y_, max = 5;
			auto lights = scene_.Lights();
			for (int i = 0; i != iterations_; ++i) {
				for (int j = 0; j != path_count; ++j) {
					double pdf_pos, pdf_dir;
					Ray light_ray;
					Spectrum Le = lights[0]->SampleLe(light_ray, rng_.Get2(), pdf_pos, pdf_dir);
					Le /= pdf_pos * pdf_dir;

					for (int bounce = 0; ; ++bounce) {
						if (bounce > max || !scene_.Intersect(light_ray)) break;

						if (!isect.bsdf_) break;

						Vector u, v, w(isect.normal_);
						MakeCoordinateSystem(w, u, v);

						const Vector tmp = -ray.direction_;
						const Vector wo = Vector(Dot(tmp, u), Dot(tmp, v), Dot(tmp, w));
					}
					Intersection isect;
					if (!scene_.Intersect(light_ray, isect)) break;

				}
			}
		}
};

} // namespace Zebra

#endif /* _BI_PATH_TRACER_HPP_ */