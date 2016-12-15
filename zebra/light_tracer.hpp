/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-13 00:23:44
**/

#ifndef _LIGHT_TRACER_HPP_
#define _LIGHT_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "isect.hpp"
#include "scene.hpp"
#include "integrator.hpp"

namespace Zebra {

class LightTracer : public Integrator
{
	public:
		LightTracer(const int iteration, const Scene &scene)
		:Integrator(scene), iteration_(iteration) { }

		std::string Render() {
			auto lights = scene_.Lights();
			for (int i = 0; i != iteration_; ++i)
				for (int x = 0; x != camera_.resolution_.x_; ++x)
					for (int y = 0; y != camera_.resolution_.y_; ++y) {
						Walk(lights[0], 1.0);
					}
			return WriteImage();
		}

		void Walk(const Light *light, double light_pdf) {
			double pdf_pos, pdf_dir;
			Point2<double> u(distribution_(generator_), distribution_(generator_));
			Ray ray;
			Spectrum l = light->SampleLe(ray, u, pdf_pos, pdf_dir);
			l *= 1.0 / (light_pdf * pdf_pos * pdf_dir);
			for (int bounce = 0; ; ++bounce) {
				Isect isect;
				if (!scene_.Intersect(ray, isect) || bounce > 0) break;

				Vector u, v, w(isect.Normal());
				if (std::fabs(w.x_) > std::fabs(w.y_))
					u = Normalize(Cross(Vector(0, 1, 0), w));
				else
					u = Normalize(Cross(Vector(1, 0, 0), w));
				v = Cross(w, u);

				const Vector tmp = -ray.Direction();
				const Vector wo = Vector(Dot(tmp, u), Dot(tmp, v), Dot(tmp, w));

				if (!isect.Bsdf()->IsDelta()) {

					// Ray ray(isect.Position() + wi * 1e-4, wi);
					// if (!scene_.IntersectP(ray, distance))
						// pixels_[camera_.RasterToIndex(raster)] += f;
				}
			}
		}

	private:
		const int iteration_;
};

} // namespace Zebra

#endif /* _LIGHT_TRACER_HPP_ */