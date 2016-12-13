/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 14:27:30
**/

#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <iostream>
#include <iomanip>
#include <cassert>

#include "vector.hpp"

namespace Zebra {

template <typename T>
class Point2
{
	public:
		T x_;
		T y_;

		Point2():x_(0), y_(0) { }
		explicit Point2(const T &t):x_(t), y_(t) { }
		Point2(const T &x, const T &y):x_(x), y_(y) { }
};

template <typename T>
class Point3
{
	public:

		T x_, y_, z_;

		Point3():x_(0), y_(0), z_(0) { }
		Point3(const T &t):x_(t), y_(t), z_(t) { }
		Point3(const T &x, const T &y, const T &z):x_(x), y_(y), z_(z) { }
		Point3(const Point3<T> &p):x_(p.x_), y_(p.y_), z_(p.z_) { }
		Point3& operator=(const Point3<T> &p) {
			x_ = p.x_;
			y_ = p.y_;
			z_ = p.z_;
			return *this;
		}

		template <typename U>
		Point3(const Vector3<U> &v):x_(v.x_), y_(v.y_), z_(v.z_) { }

		template <typename U>
		explicit operator Vector3<U>() const {
			return Vector3<U>(x_, y_, z_);
		}
		bool operator==(const Point3<T> &p) const { return x_ == p.x_ && y_ == p.y_ && z_ == p.z_; }
		bool operator!=(const Point3<T> &p) const { return !operator==(p); }

		Point3<T> operator+(const Point3<T> &p) const {
			return Point3<T>(x_ + p.x_, y_ + p.y_, z_ + p.z_);
		}
		Point3<T> operator+(const Vector3<T> &v) const {
			return Point3<T>(x_ + v.x_, y_ + v.y_, z_ + v.z_);
		}
		Point3<T>& operator+=(const Point3<T> &p) {
			x_ += p.x_;
			y_ += p.y_;
			z_ += p.z_;
			return *this;
		}
		Point3<T>& operator+=(const Vector3<T> &v) {
			x_ += v.x_;
			y_ += v.y_;
			z_ += v.z_;
			return *this;
		}
		Vector3<T> operator-(const Point3<T> &p) const {
			return Vector3<T>(x_ - p.x_, y_ - p.y_, z_ - p.z_);
		}
		Point3<T> operator-(const Vector3<T> &v) const {
			return Point3<T>(x_ - v.x_, y_ - v.y_, z_ - v.z_);
		}
		Point3<T>& operator-=(const Point3<T> &p) {
			x_ -= p.x_;
			y_ -= p.y_;
			z_ -= p.z_;
			return *this;
		}
		Point3<T>& operator-=(const Vector3<T> &v) {
			x_ -= v.x_;
			y_ -= v.y_;
			z_ -= v.z_;
			return *this;
		}

		template <typename U>
		Point3<T> operator*(const U u) const {
			return Point3<T>(x_ * u, y_ * u, z_ * u);
		}
		template <typename U>
		Point3<T>& operator*=(const U u) {
			x_ *= u;
			y_ *= u;
			z_ *= u;
			return *this;
		}
		template <typename U>
		Point3<T> operator/(const U u) const {
			double inv = 1.0 / u;
			return Point3<T>(x_ * inv, y_ * inv, z_ * inv);
		}
		template <typename U>
		Point3<T>& operator/=(const U u) {
			double inv = 1.0 / u;
			x_ *= inv;
			y_ *= inv;
			z_ *= inv;
			return *this;
		}

		Point3<T> operator-() const { return Point3<T>(-x_, -y_, -z_); }

		T& operator[](size_t i) {
			assert(i < 3);
			if (i == 0) return x_;
			if (i == 1) return y_;
			return z_;
		}
		const T& operator[](size_t i) const {
			assert(i < 3);
			if (i == 0) return x_;
			if (i == 1) return y_;
			return z_;
		}
};

typedef Point3<double> Point;
typedef Point3<double> Position;

} // namespace Zebra

#endif /* _POINT_HPP_ */