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

		virtual void SampleD(const Point &p, const Point2 &u, Vector &wi, double &pdf, double &dis)
			const = 0;

		virtual void SampleL(const Point2 &u, Point &p, Vector &normal, double &pdf) const = 0;

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

		void SampleD(const Point &p, const Point2 &u, Vector &wi, double &pdf, double &dis) const {
			Vector x, y, z(position_ - p);
			dis = z.Length();
			MakeCoordinateSystem(z, x, y);
			double cos_theta = std::sqrt(std::max(0.0, 1.0 - radius_ * radius_ / z.Length2()));
			double cosa = 1 - u.x_ + u.x_ * cos_theta;
			double sina = std::sqrt(1 - cosa * cosa);
			double phi = 2 * PI * u.y_;
			wi = Normalize(x * (std::cos(phi) * sina) + y * (std::sin(phi) * sina) + z * cosa);
			pdf = UniformConePdf(cos_theta);
		}

		void SampleL(const Point2 &u, Point &p, Vector &n, double &pdf) const override {
			p = position_ + UniformSampleSphere(u) * radius_;
			Vector tmp = p - position_;
			n = Normalize(tmp);
			double cos_theta = std::sqrt(std::max(0.0, 1.0 - radius_ * radius_ / tmp.Length2()));
			pdf = UniformConePdf(cos_theta);
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

		void SampleD(const Point &p, const Point2 &u, Vector &wi, double &pdf, double &dis) const {

		}

		void SampleL(const Point2 &u, Point &p, Vector &n, double &pdf) const {

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