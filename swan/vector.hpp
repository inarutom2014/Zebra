/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-05 14:23:02
**/

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>

namespace Swan {

template <typename T>
class Vector3
{
	public:
		T x_, y_, z_;

		Vector3():x_(0), y_(0), z_(0) { }
		explicit Vector3(const T &t):x_(t), y_(t), z_(t) { }
		Vector3(const T &x, const T &y, const T &z):x_(x), y_(y), z_(z) { }
		Vector3(const Vector3<T> &v):x_(v.x_), y_(v.y_), z_(v.z_) { }
		Vector3<T>& operator=(const Vector3<T> &v) {
			x_ = v.x_;
			y_ = v.y_;
			z_ = v.z_;
			return *this;
		}

		bool operator==(const Vector3<T> &v) const {return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;}
		bool operator!=(const Vector3<T> &v) const {return !operator==(v);}

		Vector3<T> operator+(const Vector3<T> &v) const {
			return Vector3<T>(x_ + v.x_, y_ + v.y_, z_ + v.z_);
		}
		Vector3<T> operator-(const Vector3<T> &v) const {
			return Vector3<T>(x_ - v.x_, y_ - v.y_, z_ - v.z_);
		}
		Vector3<T>& operator+=(const Vector3<T> &v) {
			x_ += v.x_, y_ += v.y_, z_ += v.z_;
			return *this;
		}
		Vector3<T>& operator-=(const Vector3<T> &v) {
			x_ -= v.x_, y_ -= v.y_, z_ -= v.z_;
			return *this;
		}

		template <typename U>
		Vector3<T> operator*(const U u) const {
			return Vector3<T>(x_ * u, y_ * u, z_ * u);
		}
		template <typename U>
		Vector3<T> operator/(const U u) const {
			double inv = 1.0 / u;
			return Vector3<T>(x_ * inv, y_ * inv, z_ * inv);
		}
		template <typename U>
		Vector3<T>& operator*=(const U u) {
			x_ *= u, y_ *= u, z_ *= u;
			return *this;
		}
		template <typename U>
		Vector3<T>& operator/=(const U u) {
			double inv = 1.0 / u;
			x_ *= inv, y_ *= inv, z_ *= inv;
			return *this;
		}

		Vector3<T> operator-() const { return Vector3<T>(-x_, -y_, -z_); }

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

		double Length2() const { return x_ * x_ + y_ * y_ + z_ * z_; }
		double Length () const { return std::sqrt(Length2()); }

		friend std::ostream& operator<<(std::ostream &os, const Vector3<T> &v) {
			return os << std::setw(8) << v.x_ << " "
								<< std::setw(8) << v.y_ << " "
								<< std::setw(8) << v.z_ << std::endl;
		}
};

template <typename T, typename U>
inline Vector3<T> operator*(const U u, const Vector3<T> &v)
{
	return v * u;
}

template <typename T>
inline T Dot(const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(
		lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
		lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
		lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_
	);
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v)
{
	double len = v.Length();
	return v / len;
}

template <typename T>
inline Vector3<T> Mult(const Vector3<T> &a, const Vector3<T> &b)
{
	return Vector3<T>(a.x_ * b.x_, a.y_ * b.y_, a.z_ * b.z_);
}

typedef Vector3<double> Vector;
typedef Vector3<double> Spectrum;

} // namespace Swan

#endif /* _VECTOR_HPP_ */