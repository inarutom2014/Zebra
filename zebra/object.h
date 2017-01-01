/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-31 23:25:32
**/

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "vector.h"
#include "ray.h"
#include "interaction.h"

const double PI = 3.141592653589793238;

namespace Zebra {

enum BSDF { Diffuse, Reflect, Refract };

class Object
{
	public:
		Object(BSDF t, const Spectrum &e, const Spectrum &c):t_(t), e_(e), c_(c) { }

		virtual bool Intersect(Ray &r, Interaction *i) const = 0;
		virtual bool IntersectP(const Ray &r) const = 0;
		virtual ~Object() { }

		Vector SampleF(const Vector &d, const Vector &n, const Vector2 &u) const {
			switch (t_) {
				case Diffuse: {
					Vector x, y, z(n);
					if (std::fabs(z.x_) > std::fabs(z.y_))
						x = Normalize(Cross(Vector(0, 1, 0), z));
					else
						x = Normalize(Cross(Vector(1, 0, 0), z));
					y = Cross(z, x);

					double m = std::sqrt(u.x_);
					double phi = 2 * PI * u.y_;
					Vector a(m * std::cos(phi), m * std::sin(phi), std::sqrt(1 - m));

					return x * a.x_ + y * a.y_ + z * a.z_;
				}
				case Reflect: {
					return Normalize(d - n * (2 * Dot(n, d)));
				}
				case Refract: {
					double cosi = Dot(n, d);
					bool entering = cosi < 0;

					cosi = entering ? -cosi : cosi;
					double etai = entering ? 1.0 : 1.3;
					double etat = entering ? 1.3 : 1.0;

					double eta = etai / etat;

					double sini = std::max(0.0, 1 - cosi * cosi);
					double sint = eta * eta * sini;

					if (sint >= 1) return Normalize(d - n * (2 * Dot(n, d)));

					double cost = std::sqrt(1 - sint);
					double term1 = etai * cost;
					double term2 = etat * cosi;
					double term3 = etai * cosi;
					double term4 = etat * cost;

					double parl = (term2 - term1) / (term2 + term1);
					double perp = (term3 - term4) / (term3 + term4);
					double re = (parl * parl + perp * perp) * 0.5;
					if (u.x_ < re)
						return Normalize(d - n * (2 * Dot(n, d)));
					else
						return Normalize(d * eta - n * ((entering ? 1 : -1) * ((eta * cosi) + cost)));
				}
				default:
					assert(0);
			}
		}

		BSDF     t_;
		Spectrum e_;
		Spectrum c_;
};

class Sphere : public Object
{
	public:
		Sphere(BSDF t, const Point &p, double r, const Spectrum &e, const Spectrum &c)
		:Object(t, e, c), p_(p), r_(r) { }

		bool Intersect(Ray &r, Interaction *i) const override {
			Vector l = p_ - r.o_;
			double s = Dot(l, r.d_);
			double l2 = l.Length2();
			double r2 = r_ * r_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			if (d > r.m_) return false;

			r.m_ = d;
			Point p = r.o_ + r.d_ * d;
			*i = Interaction(p, Normalize(p - p_), this);
			return true;
		}

		bool IntersectP(const Ray &r) const override {
			Vector l = p_ - r.o_;
			double s = Dot(l, r.d_);
			double l2 = l.Length2();
			double r2 = r_ * r_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			return d < r.m_ ? true : false;
		}

		Point    p_;
		double   r_;
};

} // namespace Zebra

#endif /* _OBJECT_H_ */