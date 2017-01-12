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
	public:
		Spectrum    weight_;
		const BSDF *bsdf_;
};

class BiPathTracer : public Integrator
{
	public:
		class LocalCoordinateSystem {
			public:
				LocalCoordinateSystem(const Vector &w):w_(w) {
					if (std::fabs(w_.x_) > std::fabs(w_.y_))
						u_ = Normalize(Cross(Vector(0, 1, 0), w_));
					else
						u_ = Normalize(Cross(Vector(1, 0, 0), w_));
					v_ = Cross(w_, u_);
				}

				Vector ToLocal(const Vector &a) {
					return Vector(Dot(a, u_), Dot(a, v_), Dot(a, w_));
				}
				Vector ToWorld(const Vector &a) {
					return u_ * a.x_ + v_ * a.y_ + w_ * a.z_;
				}

			private:
				Vector w_, u_, v_;
		};

		BiPathTracer(int iterations, const Scene &scene):Integrator(iterations, scene) { }

		std::string Render() override {
			auto beg = std::chrono::high_resolution_clock::now();
			int X = camera_.resolution_.x_, Y = camera_.resolution_.y_;
			auto lights = scene_.Lights();
			Spectrum L;
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int x = 0; x < X; ++x) {
				fprintf(stderr, "\rprogress: %.1f %%", (double)x / (X - 1) * 100);
				for (int y = 0; y < Y; ++y, L = Spectrum()) {
					for (int n = 0; n < iterations_; ++n) {
						double dx = rng_.Get1() - 0.5;
						double dy = rng_.Get1() - 0.5;
						Ray ray(Point(), camera_.RasterToWorld(Point2(dx + x, dy + y)));
						L += BDPT(lights, ray);
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
		void BDPT(const std::vector<Light *> &lights, Ray ray) {
			double pdf_pos, pdf_dir;
			Ray light_ray;
			Spectrum Le = lights[0]->SampleLe(light_ray, rng_.Get2(), pdf_pos, pdf_dir);
			Le /= pdf_pos * pdf_dir;

			// std::vector<> vertex[5];
			for (int bounce = 0; ; ++bounce) {
				Intersection isect;
				if (bounce > max || !scene_.Intersect(light_ray, isect)) break;

				if (!isect.bsdf_) break;

				LocalCoordinateSystem local(isect.normal_);
				const Vector wo = local.ToLocal(-light_ray.direction_);

				if (!isect.bsdf_->IsDelta()) {
					double pdf, distance;
					Vector dir;
					Point2i raster;
					Spectrum Wi = camera_.SampleWi(isect.position_, dir, raster, pdf, distance);
					if (!camera_.RasterIsValid(raster)) break;
					const Vector wi = local.ToLocal(dir);
					Spectrum f = isect.bsdf_->F(wo, wi);
					f *= Le * Wi * CosTheta(wi) * (1.0 / pdf);

					Ray ray(isect.position_ + dir * kEpsilon, dir, distance);
					if (!scene_.IntersectP(ray))
						pixels_[camera_.RasterToIndex(raster)] += f / iterations_;
				}

				Vector wi;
				if (!SampleNext(isect, wo, bounce, Le, wi))
					break;

				const Vector dir = local.ToWorld(wi);
				light_ray = Ray(isect.position_ + dir * kEpsilon, dir);
			}

			Spectrum L(0), weight(1);
			bool last_specular = false;
			for (int bounce = 0; ; ++bounce) {
				Intersection isect;
				bool intersect;
				if (bounce > max || !(intersect = scene_.Intersect(ray, isect))) break;

				if ((!bounce || last_specular) && intersect) {
					auto light = isect.primitive_->GetAreaLight();
					L += weight * (light ? light->L() : Spectrum());
				}

				if (!isect.bsdf_) break;

				LocalCoordinateSystem local(isect.normal_);
				const Vector wo = local.ToLocal(-light_ray.direction_);

				if (!isect.bsdf_->IsDelta()) {
					for (auto e : lights) {
						Vector dir;
						double distance, pdf;
						Spectrum l = e->SampleLi(isect.position_, rng_.Get2(), dir, distance, pdf);
						const Vector wi = local.ToLocal(dir);
						Spectrum f = isect.bsdf_->F(wo, wi);

						Ray shadow_ray(isect.position_ + dir * kEpsilon, dir, distance);
						if (!scene_.IntersectP(shadow_ray))
							L += weight * f * l * (CosTheta(wi) * (1.0 / pdf));
					}
					last_specular = false;
				} else {
					last_specular = true;
				}

				Vector wi;
				if (!SampleNext(isect, wo, bounce, weight, wi))
					break;

				const Vector dir = local.ToWorld(wi);
				ray = Ray(isect.position_ + dir * kEpsilon, dir);
			}
		}

		bool SampleNext(const Intersection &isect, const Vector &wo, int bounce,
			Spectrum &weight, Vector &wi) {
			double pdf;
			Spectrum f = isect.bsdf_->SampleF(wo, rng_.Get2(), wi, pdf);
			if (f.IsZero() || pdf == 0) return false;
			weight *= f * (AbsCosTheta(wi) * (1.0 / pdf));

			if (bounce > 3) {
				double p = std::max(f.x_, std::max(f.y_, f.z_));
				if (rng_.Get1() > p) return false;
				weight *= 1.0 / p;
			}
			return true;
		}

	const int max = 5;
};

} // namespace Zebra

#endif /* _BI_PATH_TRACER_HPP_ */