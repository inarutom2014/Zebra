/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 18:37:23
**/

#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "isect.hpp"
#include "parameter.hpp"

namespace Zebra {

class Object
{
	public:
		Object(const BSDF *bsdf):bsdf_(bsdf) { }

		virtual bool Intersect(const Ray &ray, Isect &isect) const = 0;

		virtual bool IntersectP(const Ray &ray, double distance) const = 0;

		virtual ~Object() { }

	protected:
		const BSDF *bsdf_;
};

class Sphere : public Object
{
	public:
		Sphere(double radius, const Point &position, const BSDF *bsdf)
		:Object(bsdf), radius_(radius), position_(position) { }

		bool Intersect(const Ray &r, Isect &i) const override {
			Vector l = position_ - r.Origin();
			double s = Dot(l, r.Direction());
			double l2 = l.Length2();
			double r2 = radius_ * radius_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			if (d < i.Distance()) {
				Point p(r.Origin() + r.Direction() * d);
				i.Update(d, p, Normalize(p - position_), bsdf_);
				return true;
			}
			return false;
		}

		bool IntersectP(const Ray &r, double D) const override {
			Vector l = position_ - r.Origin();
			double s = Dot(l, r.Direction());
			double l2 = l.Length2();
			double r2 = radius_ * radius_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			if (d < D) return true;
			return false;
		}

	private:
		const double radius_;
		const Point  position_;
};

class Plane : public Object
{
	public:
		Plane(const Point &position, const Vector &normal, const BSDF *bsdf)
		:Object(bsdf), position_(position), normal_(Normalize(normal)) { }

		bool Intersect(const Ray &r, Isect &i) const override {
			double a = Dot(normal_, r.Direction());
			if (a > 0) return false;

			double b = -Dot(r.Origin() - position_, normal_);

			double d = b / a;

			if (d > 0 && d < i.Distance()) {
				Point p(r.Origin() + r.Direction() * d);
				i.Update(d, p, normal_, bsdf_);
				return true;
			}
			return false;
		}

		bool IntersectP(const Ray &r, double D) const override {
			double a = Dot(normal_, r.Direction());
			if (a > 0) return false;

			double b = -Dot(r.Origin() - position_, normal_);

			double d = b / a;

			if (d > 0 && d < D) return true;
			return false;
		}

	private:
		Point  position_;
		Vector normal_;
};

Object* NewSphere(Parameter &param, const BSDF *bsdf)
{
	Point position = param.FindPosition();
	double radius  = param.FindDouble();
	return new Sphere(radius, position, bsdf);
}

Object* NewPlane(Parameter &param, const BSDF *bsdf)
{
	Point position = param.FindPosition();
	Vector normal = param.FindVector();
	return new Plane(position, normal, bsdf);
}

} // namespace Zebra

#endif /* _OBJECT_HPP_ */