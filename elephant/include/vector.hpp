/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-02 15:23:23
**/

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>

namespace Elephant {

template <typename U>
class Vector
{
	public:
		Vector() { }

		explicit Vector(size_t size, U u = 0) {
			std::vector<U> tmp(size, u);
			data_.swap(tmp);
		}

		auto size() const { return data_.size(); }
		auto begin() const { return data_.begin(); }
		auto end() const { return data_.end(); }

		U& operator[](size_t i) {
			// assert(i < size());
			return data_[i];
		}
		const U& operator[](size_t i) const {
			// assert(i < size());
			return data_[i];
		}

		Vector& operator=(const Vector &that) {
			// Validate(that);
			data_ = that.data_;
			return *this;
		}

		template<typename V>
		Vector(const Vector<V> &v) {
			size_t end = v.size();
			data_.resize(end);
			for (size_t i = 0; i != end; ++i)
				data_[i] = static_cast<U>(v.data_[i]);
		}

		template<typename V>
		Vector(const Vector<V> &v, const Vector<size_t> &index) {
			size_t end = index.size();
			data_.resize(end);
			for (size_t i = 0; i != end; ++i)
				data_[i] = static_cast<U>(v.data_[index[i]]);
		}

		bool operator==(const Vector &that) const { return data_ == that.data_; }

		bool operator!=(const Vector &that) const { return data_ != that.data_; }

		Vector operator-() const {
			size_t end = size();
			Vector<U> res(end);
			for (size_t i = 0; i != end; ++i)
				res.data_[i] = -data_[i];
			return std::move(res);
		}

		template<typename V>
		U Dot(const Vector<V> &that) const {
			// Validate(that);
			assert(size() == that.size());
			U res = 0;
			for (size_t i = 0, end = size(); i < end; ++i)
				res += data_[i] * that.data_[i];
			return res;
		}

		template<typename V>
		Vector operator+(const Vector<V> &that) const {
			// Validate(that);
			assert(size() == that.size());
			size_t end = size();
			Vector<U> res(end);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] + that.data_[i];
			return std::move(res);
		}
		template<typename V>
		Vector& operator+=(const Vector<V> &that) {
			// Validate(that);
			assert(size() == that.size());
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] += that.data_[i];
			return *this;
		}
		template<typename V>
		Vector operator+(const V &val) const {
			size_t end = size();
			Vector<U> res(end);
			U v = static_cast<U>(val);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] + v;
			return std::move(res);
		}
		template<typename V>
		Vector& operator+=(const V &val) {
			U v = static_cast<U>(val);
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] += v;
			return *this;
		}

		template<typename V>
		Vector operator-(const Vector<V> &that) const {
			// Validate(that);
			assert(size() == that.size());
			size_t end = size();
			Vector<U> res(end);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] - that.data_[i];
			return std::move(res);
		}
		template<typename V>
		Vector& operator-=(const Vector<V> &that) {
			// Validate(that);
			assert(size() == that.size());
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] -= that.data_[i];
			return *this;
		}
		template<typename V>
		Vector operator-(const V &val) const {
			size_t end = size();
			Vector<U> res(end);
			U v = static_cast<U>(val);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] - v;
			return std::move(res);
		}
		template<typename V>
		Vector& operator-=(const V &val) {
			U v = static_cast<U>(val);
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] -= v;
			return *this;
		}

		template<typename V>
		Vector operator*(const V &val) const {
			size_t end = size();
			Vector<U> res(end);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] * val;
			return std::move(res);
		}
		template<typename V>
		Vector& operator*=(const V &val) {
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] *= val;
			return *this;
		}

		template<typename V>
		Vector operator/(const Vector<V> &that) const {
			size_t end = size();
			assert(end == that.size());
			Vector<U> res(end);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] / that.data_[i];
			return std::move(res);
		}
		template<typename V>
		Vector& operator/=(const Vector<V> &that) {
			size_t end = size();
			assert(end == that.size());
			for (size_t i = 0; i < end; ++i)
				data_[i] /= that.data_[i];
			return *this;
		}
		template<typename V>
		Vector operator/(const V &val) const {
			size_t end = size();
			Vector<U> res(end);
			assert(val);
			U v = static_cast<U>(1.0 / val);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] * v;
			return std::move(res);
		}
		template<typename V>
		Vector& operator/=(const V &val) {
			assert(val);
			U v = static_cast<U>(1.0 / val);
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] *= v;
			return *this;
		}

		static Vector<size_t> Arange(size_t start, size_t end) {
			assert(end > start);
			Vector<size_t> res(end - start);
			for (size_t i = 0, j = start; j != end; ++i, ++j)
				res.data_[i] = j;
			return res;
		}

		static Vector<size_t> RandomIndex(size_t x, size_t up) {
			static std::default_random_engine generator(time(0));
			std::uniform_int_distribution<size_t> distribution(0, up - 1);
			Vector<size_t> res(x);
			for (size_t i = 0; i != x; ++i)
				res.data_[i] = distribution(generator);
			return res;
		}

		static Vector<U> Randomize(size_t x, double scale) {
			static std::default_random_engine generator(time(0));
			static std::normal_distribution<double> distribution(0, 1);
			Vector<U> res(x);
			for (size_t i = 0; i != x; ++i)
				res.data_[i] = static_cast<U>(distribution(generator) * scale);
			return std::move(res);
		}

		U Max() const {
			size_t end = size();
			assert(end);
			U max = data_[0];
			for (size_t i = 1; i != end; ++i)
				if (data_[i] > max)
					max = data_[i];
			return max;
		}

		size_t ArgMax() const {
			size_t end = size();
			assert(end);
			size_t res = 0;
			U max = data_[0];
			for (size_t i = 1; i != end; ++i)
				if (data_[i] > max) {
					max = data_[i];
					res = i;
				}
			return res;
		}

		U Sum() const {
			U sum = 0;
			std::for_each(data_.begin(), data_.end(), [&sum](const U &u) {
				sum += u;
			});
			return sum;
		}

		Vector<double> Exp() const {
			size_t end = size();
			assert(end);
			Vector<double> res(end);
			for (size_t i = 0; i != end; ++i)
				res.data_[i] = std::exp(data_[i]);
			return res;
		}

		Vector<double> Log() const {
			size_t end = size();
			assert(end);
			Vector<double> res(end);
			for (size_t i = 0; i != end; ++i)
				res.data_[i] = std::log(data_[i]);
			return res;
		}

		void Push(U u) { data_.push_back(u); }

		void Validate(size_t i) const { assert(i < size()); }

		void Validate(const Vector &that) const { assert(size() == that.size()); }

		std::vector<U> data_;
};

} // namespace Elephant

#endif /* _VECTOR_HPP_ */