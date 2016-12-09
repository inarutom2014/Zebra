/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 23:35:11
**/

#ifndef _PATH_TRACER_HPP_
#define _PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "isect.hpp"
#include "scene.hpp"
#include "integrator.hpp"

namespace Swan {

class PathTracer : public Integrator
{
	public:
		PathTracer(int samples, const Scene &scene, int max_depth)
		:Integrator(samples, scene), max_depth_(max_depth) { }

		Spectrum Li(Ray ray) const override {
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

				const Vector &d = ray.Direction();
				Vector wo = Vector(Dot(d, u), Dot(d, v), Dot(d, w));

				if (!isect.Bsdf()->IsDelta())
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

				double pdf;
				Vector wi;
				Spectrum f = isect.Bsdf()->SampleF(wo, wi, pdf);
				if (bounce > 3) {
					double p = std::max(f.x_, std::max(f.y_, f.z_));
					if (distribution(generator) < p) break;
					weight *= 1.0 / (1 - p);
				}
				weight *= f * (CosTheta(wi) * (1.0 / pdf));

				Vector dir = u * wi.x_ + v * wi.y_ + w * wi.z_;
				ray = Ray(isect.Position() + dir * 1e-4, dir);
			}
			return L;
		}

	private:
		int max_depth_;
};

} // namespace Swan

#endif /* _PATH_TRACER_HPP_ */