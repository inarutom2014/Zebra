/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
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
		PathTracer(int samples):samples_(samples) { }

		std::string Render(const std::vector<Object *> &scene) {
			auto beg = std::chrono::high_resolution_clock::now();
			int X = camera_.x_, Y = camera_.y_;
			Spectrum L;
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int x = 0; x < Y; ++x) {
				fprintf(stderr, "\rprogress: %.1f %%", (double)x / (Y - 1) * 100);
				for (int y = 0; y < X; ++y) {
					for (int sx = 0; sx < 2; ++sx) {
						for (int sy = 0; sy < 2; ++sy, L = Spectrum()) {
							for (int n = 0; n < samples_; ++n) {
								double dx = rng_.Get1() - 0.5;
								double dy = rng_.Get1() - 0.5;
								Ray ray(Point(), camera_.RasterToWorld(dx + sx + x, dy + sy + y));
								L += Li(scene, ray, 0);
							}
							pixels_[camera_.RasterToIndex(x, y)] += L / (samples_ * 4);
						}
					}
				}
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			fprintf(stderr, "\ntime:  %.2f  s\n", t);
			return WritePPM();
		}

		Spectrum Li(const std::vector<Object *> &scene, Ray ray, int depth) {
			Spectrum L(0), weight(1);
			for (int bounce = 0; bounce < 7; ++bounce) {
				bool intersect = false;
				Interaction interaction;
				for (auto e : scene)
					if (e->Intersect(ray, &interaction))
						intersect = true;

				if (!intersect) break;

				L += weight * interaction.o_->e_;

				const Spectrum &f = interaction.o_->c_;
				if (bounce >= 5) {
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