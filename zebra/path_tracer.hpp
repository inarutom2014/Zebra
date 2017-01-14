/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 23:35:11
**/

#ifndef _PATH_TRACER_HPP_
#define _PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "integrator.hpp"

namespace Zebra {

class PathTracer : public Integrator
{
	public:
		PathTracer(int iterations, const Scene &scene)
		:Integrator(iterations, scene) { }

		std::string Render() override {
			auto beg = std::chrono::high_resolution_clock::now();
			int X = camera_.resolution_.x_, Y = camera_.resolution_.y_;
			Spectrum L;
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int x = 0; x < X; ++x) {
				fprintf(stderr, "\rprogress: %.1f %%", (double)x / (X - 1) * 100);
				for (int y = 0; y < Y; ++y, L = Spectrum()) {
					for (int n = 0; n < iterations_; ++n) {
						double dx = rng_.Get1() - 0.5;
						double dy = rng_.Get1() - 0.5;
						Ray ray(Point(), camera_.RasterToWorld(Point2(dx + x, dy + y)));
						L += Li(ray);
					}
					pixels_[camera_.RasterToIndex(Point2i(x, y))] = L / iterations_;
				}
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			fprintf(stderr, "\ntime:  %.2f  s\n", t);
			return WriteBMP();
		}

	private:

		Spectrum Li(Ray ray) {
			Spectrum L(0), weight(1);
			bool last_specular = false;
			for (int bounce = 0; ; ++bounce) {
				Intersection isect;
				bool intersect;
				if (bounce > 5 || !(intersect = scene_.Intersect(ray, isect))) break;

				if ((!bounce || last_specular) && intersect) {
					auto light = isect.primitive_->GetAreaLight();
					L += weight * (light ? light->L() : Spectrum());
				}

				if (!isect.bsdf_) break;

				if (!isect.bsdf_->IsDelta()) {
					for (auto e : scene_.Lights()) {
						Vector wi;
						double distance, pdf;
						Spectrum l = e->SampleLi(isect.position_, rng_.Get2(), wi, distance, pdf);
						Spectrum f = isect.bsdf_->F(ray.direction_, wi);

						Ray shadow_ray(isect.position_ + wi * kEpsilon, wi, distance);
						if (!scene_.IntersectP(shadow_ray))
							L += weight * f * l * (Dot(isect.normal_, wi) * (1.0 / pdf));
					}
					last_specular = false;
				} else {
					last_specular = true;
				}

				double pdf;
				Vector wi;
				Spectrum f = isect.bsdf_->SampleF(ray.direction_, isect.normal_, rng_.Get2(), wi, pdf);
				if (f.IsZero() || pdf == 0) break;
				weight *= f * (std::abs(Dot(isect.normal_, wi)) * (1.0 / pdf));

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (rng_.Get1() > p) break;
					weight *= 1.0 / p;
				}

				ray = Ray(isect.position_ + wi * kEpsilon, wi);
			}
			return L;
		}
};

} // namespace Zebra

#endif /* _PATH_TRACER_HPP_ */