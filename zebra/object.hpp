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
#include "sampling.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "parameter.hpp"

namespace Zebra {

class Object
{
	public:
		virtual bool Intersect(Ray &ray, Intersection &isect) const = 0;

		virtual bool IntersectP(const Ray &ray) const = 0;

		virtual Isect Sample(const Point2 &u) const = 0;

		virtual double Pdf(const Isect &v) const = 0;

		virtual ~Object() { }
};

class Sphere : public Object
{
	public:
		Sphere(double radius, const Point &position):radius_(radius), position_(position) { }

		bool Intersect(Ray &r, Intersection &i) const override {
			Vector l = position_ - r.origin_;
			double s = Dot(l, r.direction_);
			double l2 = l.Length2();
			double r2 = radius_ * radius_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			if (d < r.max_) {
				r.max_ = d;
				Point p = r.origin_ + r.direction_ * d;
				i = Intersection(p, Normalize(p - position_));
				return true;
			}
			return false;
		}

		bool IntersectP(const Ray &r) const override {
			Vector l = position_ - r.origin_;
			double s = Dot(l, r.direction_);
			double l2 = l.Length2();
			double r2 = radius_ * radius_;
			if (s < 0 && l2 > r2)
				return false;
			double q2 = l2 - s * s;
			if (q2 > r2)
				return false;
			double q = std::sqrt(r2 - q2);
			double d = l2 > r2 ? (s - q) : (s + q);

			return d < r.max_ ? true : false;
		}

		Isect Sample(const Point2 &u) const override {
			Point p = position_ + UniformSampleSphere(u) * radius_;
			return Isect(p, Normalize(p - position_));
		}

		double Pdf(const Isect &v) const override {
			double sini = radius_ * radius_ / (v.position_ - position_).Length2();
			double cos_theta = std::sqrt(std::max(0.0, 1.0 - sini));
			return UniformConePdf(cos_theta);
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

		bool Intersect(Ray &r, Intersection &i) const override {
			double a = Dot(normal_, r.direction_);
			if (a > 0) return false;
			double b = -Dot(r.origin_ - position_, normal_);
			double d = b / a;
			if (d > 0 && d < r.max_) {
				r.max_ = d;
				i = Intersection(r.origin_ + r.direction_ * d, normal_);
				return true;
			} else {
				return false;
			}
		}

		bool IntersectP(const Ray &r) const override {
			double a = Dot(normal_, r.direction_);
			if (a > 0) return false;
			double b = -Dot(r.origin_ - position_, normal_);
			double d = b / a;
			return d < r.max_ ? true : false;
		}

		Isect Sample(const Point2 &u) const override {
			return Isect();
		}

		double Pdf(const Isect &v) const override {
			return 0.0;
		}

	private:
		Point  position_;
		Vector normal_;
};

Object* NewSphere(Parameter &param)
{
	Point position = param.FindPosition();
	std::string o(param.FindString());
	assert(o == "Radius");
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