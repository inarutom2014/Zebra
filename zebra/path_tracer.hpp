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
#include "isect.hpp"
#include "integrator.hpp"

namespace Zebra {

class PathTracer : public Integrator
{
	public:
		PathTracer(int samples, const Scene &scene)
		:Integrator(samples, scene), max_depth_(5) { }

		std::string Render() override {
			auto beg = std::chrono::high_resolution_clock::now();
			int X = camera_.resolution_.x_, Y = camera_.resolution_.y_;
			Spectrum L;
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int x = 0; x < X; ++x) {
				fprintf(stderr, "\rprogress: %3.1f %%", ((float)100 * x / (X - 1)));
				for (int y = 0; y < Y; ++y, L = Spectrum()) {
					for (int n = 0; n < samples_; ++n) {
						double dx = distribution(generator) - 0.5;
						double dy = distribution(generator) - 0.5;
						Ray ray(Point(), camera_.RasterToWorld(Point2<double>(dx + x, dy + y)));
						L += Li(ray);
					}
					pixels_[camera_.RasterToIndex(Point2<int>(x, y))] = L / samples_;
				}
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			std::cerr << "\ntime:  " << t << "  s\n";
			return WriteImage();
		}

		Spectrum Li(Ray ray) {
			Spectrum L(0), weight(1);
			for (int bounce = 0; ; ++bounce) {
				Isect isect;
				if (!scene_.Intersect(ray, isect) || bounce > max_depth_) break;

				Vector u, v, w(isect.Normal());
				if (std::fabs(w.x_) > std::fabs(w.y_))
					u = Normalize(Cross(Vector(0, 1, 0), w));
				else
					u = Normalize(Cross(Vector(1, 0, 0), w));
				v = Cross(w, u);

				const Vector tmp = -ray.Direction();
				const Vector wo = Vector(Dot(tmp, u), Dot(tmp, v), Dot(tmp, w));

				if (!isect.Bsdf()->IsDelta()) {
					for (auto e : scene_.Lights()) {
						Vector dir;
						double distance, pdf;
						Spectrum l = e->SampleLi(isect.Position(), dir, distance, pdf);
						Vector wi = Vector(Dot(dir, u), Dot(dir, v), Dot(dir, w));
						Spectrum f = isect.Bsdf()->F(wo, wi);

						Ray shadow_ray(isect.Position() + dir * 1e-4, dir);
						if (!scene_.IntersectP(shadow_ray, distance))
							L += weight * f * l * (CosTheta(wi) * (1.0 / pdf));
					}
				}

				double pdf;
				Vector wi;
				Spectrum f = isect.Bsdf()->SampleF(wo, wi, pdf);
				if (f.IsZero() || pdf == 0) break;
				weight *= f * (AbsCosTheta(wi) * (1.0 / pdf));

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (distribution(generator) > p) break;
					weight *= 1.0 / p;
				}

				Vector dir = u * wi.x_ + v * wi.y_ + w * wi.z_;
				ray = Ray(isect.Position() + dir * 1e-4, dir);
			}
			return L;
		}

	private:
		int max_depth_;
};

} // namespace Zebra

#endif /* _PATH_TRACER_HPP_ */