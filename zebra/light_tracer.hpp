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
#include "intersection.hpp"
#include "scene.hpp"
#include "integrator.hpp"

namespace Zebra {

class LightTracer : public Integrator
{
	public:
		LightTracer(int iterations, const Scene &scene)
		:Integrator(iterations, scene) { }

		std::string Render() {
			auto beg = std::chrono::high_resolution_clock::now();
			path_count_ = camera_.resolution_.x_ * camera_.resolution_.y_;
			auto lights = scene_.Lights();
			#pragma omp parallel for schedule(dynamic, 1)
			for (int i = 0; i < iterations_; ++i) {
				fprintf(stderr, "\rrunning iteration %d/%d", i + 1, iterations_);
				for (int j = 0; j < path_count_; ++j)
					Walk(lights[0]);
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			fprintf(stderr, "\ntime:  %.2f  s\n", t);
			return WriteBMP();
		}

		void Walk(const Light *light) {
			double pdf_pos, pdf_dir;
			Ray ray;
			Vector normal;
			Spectrum Le = light->SampleLe(ray, normal, rng_.Get2(), pdf_pos, pdf_dir);
			Le /= pdf_pos * pdf_dir;
			for (int bounce = 0; ; ++bounce) {
				Intersection isect;
				if (bounce > 5 || !scene_.Intersect(ray, isect)) break;

				if (!isect.bsdf_) break;

				if (!isect.bsdf_->IsDelta()) {
					double pdf, distance;
					Vector wi;
					Point2i raster;
					Spectrum Wi = camera_.SampleWi(isect.position_, wi, raster, pdf, distance);
					if (!camera_.RasterIsValid(raster)) break;
					Spectrum f = isect.bsdf_->F(ray.direction_, wi);
					f *= Le * Wi * Dot(isect.normal_, wi) * (1.0 / pdf);

					Ray ray(isect.position_ + wi * kEpsilon, wi, distance);
					if (!scene_.IntersectP(ray))
						pixels_[camera_.RasterToIndex(raster)] += f / iterations_;
				}

				double pdf;
				Vector wi;
				Spectrum f = isect.bsdf_->SampleF(ray.direction_, isect.normal_, rng_.Get2(), wi, pdf);
				Le *= f * (std::abs(Dot(isect.normal_, wi)) * (1.0 / pdf));

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (rng_.Get1() > p) break;
					Le *= 1.0 / p;
				}

				ray = Ray(isect.position_ + wi * kEpsilon, wi);
			}
		}

	private:
		int path_count_;
};

} // namespace Zebra

#endif /* _LIGHT_TRACER_HPP_ */