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
		LightTracer(int samples, const Scene &scene)
		:Integrator(samples, scene), max_depth_(5) { }

		std::string Render() {
			for (int i = 0; i != samples_; ++i) {
				double x = distribution_(generator_);
				auto lights = scene_.Lights();
				double pdf = 1.0 / lights.size();
				const Light *light = lights[(int)(x * lights.size())];
				Walk(light, pdf);
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
					Vector wi = camera_.DirectionToCamera(isect.Position());
					Point2<int> raster = camera_.WorldToRaster(wi / wi.z_);
					if (!camera_.RasterIsValid(raster)) break;
					double distance = wi.Length();
					wi /= wi.Length();
					double cosi = Dot(Vector(0, 0, -1), -wi);
					Spectrum f = isect.Bsdf()->F(wo, wi);
					double pdf = distance * distance /
						(camera_.resolution_.x_ * camera_.resolution_.y_ * cosi * cosi *cosi);
					f *= l * Dot(wi, isect.Normal()) * (1.0 / (camera_.ssresolution_.x_ * camera_.resolution_.y_ * pdf));
					Ray ray(isect.Position() + wi * 1e-4, wi);
					if (!scene_.IntersectP(ray, distance))
						pixels_[camera_.RasterToIndex(raster)] += f;
				}

				// double pdf;
				// Vector wi;
				// Spectrum f = isect.Bsdf()->SampleF(wo, wi, pdf);
				// if (f.IsZero() || pdf == 0) break;
				// weight *= f * (CosTheta(wi) * (1.0 / pdf));

				// if (bounce > 3) {
				// 	double p = std::max(f.x_, std::max(f.y_, f.z_));
				// 	if (distribution(generator) < p) break;
				// 	weight *= 1.0 / (1 - p);
				// }

				// Vector dir = u * wi.x_ + v * wi.y_ + w * wi.z_;
				// ray = Ray(isect.Position() + dir * 1e-4, dir);
			}
		}

	private:
		int max_depth_;
};

} // namespace Zebra

#endif /* _LIGHT_TRACER_HPP_ */