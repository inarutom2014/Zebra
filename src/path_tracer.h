/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Zebra
 *    > Created Time:  2016-12-31 23:58:47
**/

#ifndef _PATH_TRACER_H_
#define _PATH_TRACER_H_

#include <chrono>

#include "vector.h"
#include "ray.h"
#include "object.h"
#include "interaction.h"
#include "integrator.h"

namespace Zebra {

class PathTracer : public Integrator
{
	public:
		PathTracer(int samples):samples_(samples / 4) { }

		std::string Render(const std::vector<Object *> &scene) {
			auto beg = std::chrono::high_resolution_clock::now();
			int X = camera_.x_, Y = camera_.y_;
			Spectrum L;
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int y = 0; y < Y; ++y) {
				fprintf(stderr, "\rprogress: %.1f%%", (double)y / (Y - 1) * 100);
				for (int x = 0; x < X; ++x) {
					for (int sx = 0; sx < 2; ++sx) {
						for (int sy = 0; sy < 2; ++sy, L = Spectrum()) {
							for (int n = 0; n < samples_; ++n) {
								double a = 2 * rng_.Get1(), dx = a < 1 ? std::sqrt(a)-1: 1-std::sqrt(2-a);
								double b = 2 * rng_.Get1(), dy = b < 1 ? std::sqrt(b)-1: 1-std::sqrt(2-b);
								Ray ray(Point(0), camera_.RasterToWorld((dx+sx+0.5)/2+x, (dy+sy+0.5)/2+y));
								L += Li(scene, ray);
							}
							pixels_[camera_.RasterToIndex(x, y)] += L / (samples_ * 4);
						}
					}
				}
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			fprintf(stderr, "\ntime:  %.2f  s\n", t);
			return WriteBMP(samples_);
		}

		Spectrum Li(const std::vector<Object *> &scene, Ray ray) {
			double specular = 1;
			Spectrum L(0), weight(1);
			for (int bounce = 0; bounce <= 5; ++bounce) {
				bool intersect = false;
				Interaction interaction;
				for (auto e : scene)
					if (e->Intersect(ray, &interaction))
						intersect = true;

				if (!intersect) break;

				L += weight * interaction.o_->e_ * specular;

				if (Dot(ray.d_, interaction.n_) > 0) interaction.n_ = -interaction.n_;

				const Spectrum &f = interaction.o_->c_;

				#ifdef DI
				if (!interaction.o_->IsDelta()) {
					for (auto e : scene) {
						if (!e->IsLight()) continue;
						double pdf, dis;
						Vector nd = e->SampleLi(interaction.p_, rng_.Get2(), &pdf, &dis);
						Ray shadow_ray(interaction.p_ + nd * 1e-4, nd, dis);
						bool flag = false;
						for (auto ee : scene) {
							if (ee != e && ee->IntersectP(shadow_ray)) {
								flag = true;
								break;
							}
						}
						if (!flag)
							L += weight * f * e->e_ * (Dot(nd, interaction.n_) * pdf);
					}
					specular = 0;
				} else {
					specular = 1;
				}
				#endif

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (rng_.Get1() > p) break;
					weight /= p;
				}

				weight *= f;

				Vector nd = interaction.o_->SampleF(ray.d_, interaction.n_, rng_.Get2());

				ray.m_ = 1e10;
				ray.o_ = interaction.p_ + nd * 1e-4;
				ray.d_ = nd;
			}
			return L;
		}

	private:
		const int samples_;
};

} // namespace Zebra

#endif /* _PATH_TRACER_H_ */