/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-31 22:52:11
**/

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cmath>

namespace Zebra {

class Vector2
{
	public:
		Vector2(double x, double y):x_(x), y_(y) { }

		double x_, y_;
};

class Vector
{
	public:
		Vector():x_(0), y_(0), z_(0) { }
		Vector(double v):x_(v), y_(v), z_(v) { }
		Vector(double x, double y, double z):x_(x), y_(y), z_(z) { }
		Vector(const Vector &) = default;

		Vector operator+(const Vector &that) const {
			return Vector(x_ + that.x_, y_ + that.y_, z_ + that.z_);
		}
		Vector& operator+=(const Vector &that) {
			x_ += that.x_;
			y_ += that.y_;
			z_ += that.z_;
			return *this;
		}
		Vector operator-(const Vector &that) const {
			return Vector(x_ - that.x_, y_ - that.y_, z_ - that.z_);
		}
		Vector& operator-=(const Vector &that) {
			x_ -= that.x_;
			y_ -= that.y_;
			z_ -= that.z_;
			return *this;
		}
		Vector operator*(const Vector &that) const {
			return Vector(x_ * that.x_, y_ * that.y_, z_ * that.z_);
		}
		Vector& operator*=(const Vector &that) {
			x_ *= that.x_;
			y_ *= that.y_;
			z_ *= that.z_;
			return *this;
		}
		Vector operator*(double v) const {
			return Vector(x_ * v, y_ * v, z_ * v);
		}
		Vector& operator*=(double v) {
			x_ *= v;
			y_ *= v;
			z_ *= v;
			return *this;
		}

		Vector operator/(double v) const {
			return *this * (1.0 / v);
		}
		Vector operator/=(double v) {
			return *this *= (1.0 / v);
		}

		Vector operator-() const { return Vector(-x_, -y_, -z_); }

		double Length2() const {
			return x_ * x_ + y_ * y_ + z_ * z_;
		}
		double Length() const {
			return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
		}
		void Clamp() {
			x_ = std::min(1.0, std::max(0.0, x_));
			y_ = std::min(1.0, std::max(0.0, y_));
			z_ = std::min(1.0, std::max(0.0, z_));
		}

		double x_, y_, z_;
};

typedef Vector Point;
typedef Vector Spectrum;

inline Vector Normalize(const Vector &v) { return v / v.Length(); }

inline Vector Cross(const Vector &lhs, const Vector &rhs) {
	return Vector(
		lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
		lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
		lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_
	);
}

inline double Dot(const Vector &lhs, const Vector &rhs) {
	return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

} // namespace Zebra

#endif /* _VECTOR_H_ */