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
#include "camera.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "integrator.hpp"

namespace Zebra {

class Vertex
{
	public:
		enum Type { CAMERA, SURFACE, LIGHT };

		Vertex() { }
		Vertex(Type type, const Vector &normal, const Spectrum &weight, const Light *light)
		:type_(type), direction_(normal), normal_(normal), weight_(weight), light_(light) { }

		Vertex(Type type, const Point &position, const Vector &direction, const Vector &normal,
			const Spectrum &weight, double pdf_fwd, const Light *light)
		:type_(type), position_(position), direction_(direction), normal_(normal),
		 weight_(weight), light_(light), pdf_fwd_(pdf_fwd) { }

		Vertex(Type type, const Point &position, const Vector &direction, const Spectrum &weight,
			const Camera *camera)
		:type_(type), position_(position), direction_(direction), weight_(weight),
		 camera_(camera) { }

		Vertex(Type type, const Point &position, const Spectrum &weight, const Camera *camera)
		:type_(type), position_(position), weight_(weight), camera_(camera) { }

		Vertex(Type type, const Point &position, const Vector &direction, const Vector &normal,
			const Spectrum &weight, const Primitive *primitive, double pdf, const Vertex &prev)
		:type_(type), position_(position), direction_(direction), normal_(normal), weight_(weight),
		 primitive_(primitive), pdf_fwd_(ConvertDensity(pdf, prev)) { }

		bool IsLight() const {
			return type_ == LIGHT || (type_ == SURFACE && primitive_->GetAreaLight());
		}
		Spectrum Le(const Vertex &v) const {
			if (type_ == SURFACE) {
				const AreaLight *light = primitive_->GetAreaLight();
				return light->L();
			}
			return Spectrum();
		}
		bool IsConnectible() const {
			if (type_ == SURFACE) return primitive_->bsdf_ && !primitive_->bsdf_->IsDelta();
			return true;
		}

		Spectrum F(const Vertex &v) const {
			Vector wi = Normalize(v.position_ - position_);
			if (type_ == SURFACE && primitive_->bsdf_) primitive_->bsdf_->F(direction_, wi);
			return Spectrum(0);
		}

		double PdfLightOrigin(const Vertex &v) const {
			Vector dir = Normalize(v.position_ - position_);
			const Light *light = (type_ == LIGHT) ? light_ : primitive_->GetAreaLight();
			double pdf_pos, pdf_dir;
			light->PdfLe(Ray(position_, dir), normal_, pdf_pos, pdf_dir);
			return pdf_pos;
		}

		double ConvertDensity(double pdf, const Vertex &next) const {
			Vector dir = next.position_ - position_;
			double inv_dist2 = 1.0 / dir.Length2();
			if (next.type_ == SURFACE)
				pdf *= std::abs(Dot(next.normal_, dir * std::sqrt(inv_dist2)));
			return pdf * inv_dist2;
		}

		Type        type_;
		Point       position_;
		Vector      direction_;
		Vector      normal_;
		Spectrum    weight_;
		union {
			const Light      *light_;
			const Camera     *camera_;
			const Primitive  *primitive_;
		};
		double      pdf_fwd_ = 0;
		double      pdf_rev_ = 0;
		bool        delta_   = false;
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
						Vertex light_path[max + 1];
						Vertex camera_path[max + 2];
						int l = GenerateLightPath(lights, light_path);
						int c = GenerateCameraPath(lights, Point2i(x, y), camera_path);
						for (int i = 0; i != c; ++i) {
							for (int j = 0; j != l; ++j) {
								int depth = i + j - 2;
								if ((i == 1 && j == 1) || depth < 0 || depth > max)
									continue;
								L += ConnectBDPT(lights, j, i, light_path, camera_path);
							}
						}
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
		double MisWeight(const Vertex &sampled, int l, int c,
			Vertex *light_path, Vertex *camera_path) {

			return 0.0;
		}

		Spectrum G(const Vertex &v0, const Vertex &v1) {

			return Spectrum();
		}

		Spectrum ConnectBDPT(const std::vector<Light *> &lights, int l, int c,
			Vertex *light_path, Vertex *camera_path) {
			Spectrum L(0);
			Vertex sampled;
			if (l == 0) {

				const Vertex &pt = camera_path[c - 1];
				if (pt.IsLight()) L = pt.Le(camera_path[c - 2]) * pt.weight_;
			} else if (c == 1) {
				const Vertex &qs = light_path[l-1];
				if (qs.IsConnectible()) {
					double pdf, distance;
					Vector dir;
					Point2i raster;
					Spectrum Wi = camera_.SampleWi(qs.position_, dir, raster, pdf, distance);
					Ray shadow_ray(qs.position_ + dir * kEpsilon, dir, distance);
					if (!scene_.IntersectP(shadow_ray)) {
						sampled = Vertex(Vertex::CAMERA, Point(), Wi / pdf, &camera_);
						L = qs.weight_ * qs.F(sampled) * sampled.weight_;
						if (qs.type_ == Vertex::SURFACE) L *= std::abs(Dot(dir, qs.normal_));
					}
				}
			} else if (l == 1) {
				const Vertex &pt = camera_path[c - 1];
				double dis, pdf;
				Vector dir;
				Spectrum Li = lights[0]->SampleLi(pt.position_, rng_.Get2(), dir, dis, pdf);
				Ray shadow_ray(pt.position_ + dir * kEpsilon, dir, dis);
				if (!scene_.IntersectP(shadow_ray)) {
					sampled = Vertex(Vertex::LIGHT, dir, Li / pdf, lights[0]);
					sampled.pdf_fwd_ = sampled.PdfLightOrigin(pt);
					L = pt.weight_ * pt.F(sampled) * sampled.weight_;
					if (pt.type_ == Vertex::SURFACE) L *= std::abs(Dot(dir, pt.normal_));
				}
			} else {
				const Vertex &qs = light_path[l - 1], &pt = camera_path[c - 1];
				if (qs.IsConnectible() && pt.IsConnectible()) {
					L = qs.weight_ * qs.F(pt) * pt.F(qs) * pt.weight_;
					L *= G(qs, pt);
				}
			}
			double mis_weight = MisWeight(sampled, l, c, light_path, camera_path);
			L *= mis_weight;
			return L;
		}

		int GenerateLightPath(const std::vector<Light *> &lights, Vertex *path) {
			double pdf_pos, pdf_dir;
			Ray ray;
			Vector normal;
			Spectrum Le = lights[0]->SampleLe(ray, normal, rng_.Get2(), pdf_pos, pdf_dir);
			Spectrum weight = Le * Dot(normal, ray.direction_) / (pdf_pos * pdf_dir);
			path[0] = Vertex(Vertex::LIGHT, ray.origin_, ray.direction_, normal, weight, pdf_pos,
				lights[0]);
			return RandomWalk(lights, ray, Le, pdf_dir, path + 1) + 1;
		}

		int GenerateCameraPath(const std::vector<Light *> &lights, const Point2i &pixel,
			Vertex *path) {
			double dx = rng_.Get1() - 0.5;
			double dy = rng_.Get1() - 0.5;
			Ray ray(Point(), camera_.RasterToWorld(Point2(dx + pixel.x_, dy + pixel.y_)));
			double pdf_dir;
			camera_.PdfWe(ray, pdf_dir);
			Spectrum weight(1);
			path[0] = Vertex(Vertex::CAMERA, ray.origin_, ray.direction_, weight, &camera_);
			return RandomWalk(lights, ray, weight, pdf_dir, path + 1) + 1;
		}

		int RandomWalk(const std::vector<Light *> &lights, Ray ray, Spectrum weight, double pdf,
			Vertex *path) {
			double pdf_fwd = pdf, pdf_rev = 0;
			int bounce = 0;
			for (;;) {
				Intersection isect;
				if (!scene_.Intersect(ray, isect)) break;

				Vertex &vertex = path[bounce], &prev = path[bounce-1];
				vertex = Vertex(Vertex::SURFACE, isect.position_, ray.direction_, isect.normal_, weight,
					isect.primitive_, pdf_fwd, prev);

				if (!isect.bsdf_ || ++bounce >= max) break;

				Vector wi;
				Spectrum f = isect.bsdf_->SampleF(ray.direction_, isect.normal_, rng_.Get2(), wi,
					pdf_fwd);
				weight *= f * (std::abs(Dot(isect.normal_, wi)) * (1.0 / pdf_fwd));
				pdf_rev = isect.bsdf_->Pdf(wi, ray.direction_, isect.normal_);
				if (isect.bsdf_->IsDelta()) {
					vertex.delta_ = true;
					pdf_fwd = 0, pdf_rev = 0;
				}

				ray = Ray(isect.position_ + wi * kEpsilon, wi);

				prev.pdf_rev_ = vertex.ConvertDensity(pdf_rev, prev);
			}
			return bounce;
		}

	const int max = 5;
};

} // namespace Zebra

#endif /* _BI_PATH_TRACER_HPP_ */