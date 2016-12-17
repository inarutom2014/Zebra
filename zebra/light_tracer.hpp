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
					Walk(lights[0], 1.0);
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			fprintf(stderr, "\ntime:  %.2f  s\n", t);
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
				if (bounce > 5 || !scene_.Intersect(ray, isect)) break;

				Vector u, v, w(isect.Normal());
				if (std::fabs(w.x_) > std::fabs(w.y_))
					u = Normalize(Cross(Vector(0, 1, 0), w));
				else
					u = Normalize(Cross(Vector(1, 0, 0), w));
				v = Cross(w, u);

				const Vector tmp = -ray.Direction();
				const Vector wo = Vector(Dot(tmp, u), Dot(tmp, v), Dot(tmp, w));

				if (!isect.Bsdf()->IsDelta()) {
					Vector d = camera_.DirectionToCamera(isect.Position());
					double distance = d.Length();
					d /= distance;
					auto raster = camera_.WorldToRaster(d);
					if (!camera_.RasterIsValid(raster)) break;
					double cosi = Dot(Vector(0, 0, -1), -d);
					Vector wi = Vector(Dot(d, u), Dot(d, v), Dot(d, w));
					Spectrum f = isect.Bsdf()->F(wo, wi);
					double pdf = cosi * cosi * cosi * distance * distance;
					f *= l * CosTheta(wi) * (1.0 / pdf);

					Ray ray(isect.Position() + d * 1e-4, d);
					if (!scene_.IntersectP(ray, distance))
						pixels_[camera_.RasterToIndex(raster)] += f / iterations_;
				}

				double pdf;
				Vector wi;
				Spectrum f = isect.Bsdf()->SampleF(wo, wi, pdf);
				if (f.IsZero() || pdf == 0) break;
				l *= f * (AbsCosTheta(wi) * (1.0 / pdf));

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (distribution(generator) > p) break;
					l *= 1.0 / p;
				}

				Vector dir = u * wi.x_ + v * wi.y_ + w * wi.z_;
				ray = Ray(isect.Position() + dir * 1e-4, dir);
			}
		}

	private:
		int path_count_;
};

} // namespace Zebra

#endif /* _LIGHT_TRACER_HPP_ */