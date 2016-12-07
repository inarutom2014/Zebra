/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-02 15:23:23
**/

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <ctime>

namespace Bunny {

template<typename X> class Matrix;

template <typename U>
class Vector
{
	friend class Matrix<U>;

	public:

		Vector() { }

		explicit Vector(size_t size, U u = 0) {
			std::vector<U> tmp(size, u);
			data_.swap(tmp);
		}

		auto size() const { return data_.size(); }
		auto begin() const { return data_.begin(); }
		auto end() const { return data_.end(); }
		auto rbegin() const { return data_.rbegin(); }
		auto rend() const { return data_.rend(); }

		U& operator[](size_t i) {
			// Validate(i);
			assert(i < size());
			return data_[i];
		}
		const U& operator[](size_t i) const {
			// Validate(i);
			assert(i < size());
			return data_[i];
		}

		Vector& operator=(const Vector &that) {
			// Validate(that);
			data_ = that.data_;
			return *this;
		}
		Vector(const Vector &that):data_(that.data_) { }

		bool operator==(const Vector &that) const {
			return data_ == that.data_;
		}

		bool operator!=(const Vector &that) const {
			return data_ != that.data_;
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
		Vector operator+(const V &v) const {
			size_t end = size();
			Vector<U> res(end);
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] + v;
			return std::move(res);
		}
		template<typename V>
		Vector& operator+=(const V &v) {
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
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] - val;
			return std::move(res);
		}
		template<typename V>
		Vector& operator-=(const V &val) {
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] -= val;
			return *this;
		}

		// template<typename V>
		// Vector operator*(const Vector<V> &that) const {
		// 	// Validate(that);
		// 	assert(size() == that.size());
		// 	size_t end = size();
		// 	Vector<U> res(end);
		// 	for (size_t i = 0; i < end; ++i)
		// 		res.data_[i] = data_[i] * that.data_[i];
		// 	return std::move(res);
		// }
		// template<typename V>
		// Vector& operator*=(const Vector<V> &that) {
		// 	// Validate(that);
		// 	for (size_t i = 0, end = size(); i < end; ++i)
		// 		data_[i] *= that.data_[i];
		// 	return *this;
		// }
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
		Vector operator/(const V &val) const {
			size_t end = size();
			Vector<U> res(end);
			assert(val);
			V v = 1 / val;
			for (size_t i = 0; i < end; ++i)
				res.data_[i] = data_[i] * v;
			return std::move(res);
		}

		template<typename V>
		Vector& operator/=(const V &val) {
			assert(val);
			V v = 1 / val;
			for (size_t i = 0, end = size(); i < end; ++i)
				data_[i] *= v;
			return *this;
		}

		static Vector<size_t> Arange(size_t start, size_t end) {
			assert(end > start);
			Vector<size_t> res(Vector(end - start));
			for (size_t i = 0, j = start; j != end; ++i, ++j)
				res.data_[i] = j;
			return std::move(res);
		}

		void Assign(size_t i, U u) { data_[i] = u; }
		void Resize(size_t size) { data_.resize(size); }
		void Push(U u) { data_.push_back(u); }

		void Randomize(size_t size, size_t up) {
			assert(this->size() == size);
			static std::default_random_engine generator(time(0));
			static std::uniform_int_distribution<size_t> distribution(0, up-1);
			for (size_t i = 0; i != size; ++i)
				data_[i] = distribution(generator);
		}

		void Randomize(double scale) {
			assert(size());
			std::default_random_engine generator(time(0));
			std::normal_distribution<double> distribution(0, 1);
			for (size_t i = 0, size = this->size(); i != size; ++i)
				data_[i] = static_cast<U>(distribution(generator) * scale);
		}

		size_t ArgMax() const {
			assert(size());
			size_t res = 0;
			U max = data_[0];
			for (size_t i = 1, end = size(); i != end; ++i)
				if (data_[i] > max) {
					max = data_[i];
					res = i;
				}
			return res;
		}

		void Validate(size_t i) const { assert(i < size()); }

		void Validate(const Vector &that) const { assert(size() == that.size()); }

		std::vector<U> data_;
};

} // namespace Bunny

#endif /* _VECTOR_HPP_ */