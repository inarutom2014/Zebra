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
		virtual bool Intersect(const Ray &ray, Isect &isect) const = 0;

		virtual bool IntersectP(const Ray &ray, double distance) const = 0;

		virtual ~Object() { }
};

class Sphere : public Object
{
	public:
		Sphere(double radius, const Point &position):radius_(radius), position_(position) { }

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

			if (d < r.Distance()) {
				r.SetDistance(d);
				i = Isect(d, r.Origin() + r.Direction() * d, Normalize(p - position_));
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

			return d < D ? true : false;
		}

	private:
		const double radius_;
		const Point  position_;
};

class Plane : public Object
{
	public:
		Plane(const Point &position, const Vector &normal)
		:position_(position), normal_(Normalize(normal)) { }

		bool Intersect(const Ray &r, Isect &i) const override {
			double a = Dot(normal_, r.Direction());
			if (a > 0) return false;

			double b = -Dot(r.Origin() - position_, normal_);

			double d = b / a;

			if (d > 0 && d < r.Distance()) {
				r.SetDistance(d);
				i = Isect(d, r.Origin() + r.Direction() * d, normal_);
				return true;
			} else {
				return false;
			}
		}

		bool IntersectP(const Ray &r, double D) const override {
			double a = Dot(normal_, r.Direction());
			if (a > 0) return false;

			double b = -Dot(r.Origin() - position_, normal_);

			double d = b / a;

			return d < D ? true : false;
		}

	private:
		Point  position_;
		Vector normal_;
};

Object* NewSphere(Parameter &param)
{
	Point position = param.FindPosition();
	double radius  = param.FindDouble();
	return new Sphere(radius, position);
}

Object* NewPlane(Parameter &param)
{
	Point position = param.FindPosition();
	Vector normal = param.FindVector();
	return new Plane(position, normal);
}

} // namespace Zebra

#endif /* _OBJECT_HPP_ */