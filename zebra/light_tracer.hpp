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
			return WritePNG();
		}

		void Walk(const Light *light) {
			double pdf_pos, pdf_dir;
			Ray ray;
			Spectrum l = light->SampleLe(ray, rng_.Get2D(), pdf_pos, pdf_dir);
			l *= 1.0 / (pdf_pos * pdf_dir);
			for (int bounce = 0; ; ++bounce) {
				Intersection isect;
				if (bounce > 5 || !scene_.Intersect(ray, isect)) break;

				if (!isect.bsdf_) break;

				Vector u, v, w(isect.normal_);
				if (std::fabs(w.x_) > std::fabs(w.y_))
					u = Normalize(Cross(Vector(0, 1, 0), w));
				else
					u = Normalize(Cross(Vector(1, 0, 0), w));
				v = Cross(w, u);

				const Vector tmp = -ray.direction_;
				const Vector wo = Vector(Dot(tmp, u), Dot(tmp, v), Dot(tmp, w));

				if (!isect.bsdf_->IsDelta()) {
					Vector d = camera_.DirectionToCamera(isect.position_);
					double distance = d.Length();
					d /= distance;
					auto raster = camera_.WorldToRaster(d);
					if (!camera_.RasterIsValid(raster)) break;
					double cosi = Dot(Vector(0, 0, -1), -d);
					Vector wi = Vector(Dot(d, u), Dot(d, v), Dot(d, w));
					Spectrum f = isect.bsdf_->F(wo, wi);
					double pdf = (distance * distance * cosi * cosi * cosi) /
						(camera_.image_distance_ * camera_.image_distance_);
					f *= l * CosTheta(wi) * (1.0 / (pdf * path_count_));

					Ray ray(isect.position_ + d * kEpsilon, d, distance);
					if (!scene_.IntersectP(ray))
						pixels_[camera_.RasterToIndex(raster)] += f / iterations_;
				}

				double pdf;
				Vector wi;
				Spectrum f = isect.bsdf_->SampleF(wo, rng_.Get2D(), wi, pdf);
				l *= f * (AbsCosTheta(wi) * (1.0 / pdf));

				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (rng_.Get1D() > p) break;
					l *= 1.0 / p;
				}

				Vector dir = u * wi.x_ + v * wi.y_ + w * wi.z_;
				ray = Ray(isect.position_ + dir * kEpsilon, dir);
			}
		}

	private:
		int path_count_;
};

} // namespace Zebra

#endif /* _LIGHT_TRACER_HPP_ */