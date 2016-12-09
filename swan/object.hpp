/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 18:37:23
**/

#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "isect.hpp"
#include "parameter.hpp"

namespace Swan {

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

		bool Intersect(const Ray &ray, Isect &isect) const override {
			Vector l = position_ - ray.Origin();
			double s = Dot(l, ray.Direction());
			double l2 = l.Length2();
			double r2 = radius_ * radius_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			if (d < isect.Distance()) {
				Point p(ray.Origin() + ray.Direction() * d);
				isect.Update(d, p, Normalize(p - position_), bsdf_);
				return true;
			}
			return false;
		}

		bool IntersectP(const Ray &ray, double D) const override {
			Vector l = position_ - ray.Origin();
			double s = Dot(l, ray.Direction());
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

Object* NewSphere(Parameter &param, const BSDF *bsdf)
{
	Point position = param.FindPosition();
	double radius  = param.FindDouble();
	return new Sphere(radius, position, bsdf);
}

} // namespace Swan

#endif /* _OBJECT_HPP_ */