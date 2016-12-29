/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-11-29 21:07:47
**/

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <random>
#include <ctime>

#include "vector.hpp"

namespace Elephant {

template <typename U>
class Matrix
{
	public:
		Matrix() { }

		Matrix(int row, int col, U val = 0) {
			assert(row > 0 && col > 0);
			data_ = std::vector<Vector<U>>(row, Vector<U>(col, val));
		}

		Matrix(const std::pair<size_t, size_t> &shape)
		:data_(std::vector<Vector<U>>(shape.first, Vector<U>(shape.second))) { }

		Matrix(const Matrix &) = default;
		Matrix& operator=(const Matrix &) = default;

		template<typename V>
		Matrix(const Matrix<V> &matrix, const Vector<size_t> &index) {
			size_t end = index.size();
			data_.resize(end);
			for (size_t i = 0; i != end; ++i)
				data_[i] = static_cast<Vector<U>>(matrix[index[i]]);
		}

		template<typename V>
		Matrix& operator=(const Matrix<V> &that) {
			size_t end = that.Row();
			data_.resize(end);
			for (size_t i = 0; i != end; ++i)
				data_[i] = that[i];
			return *this;
		}

		Matrix T() const {
			size_t row = Row();
			size_t col = Col();
			Matrix<U> res(col, row);
			for (size_t i = 0; i != row; ++i)
				for (size_t j = 0; j != col; ++j)
					res.data_[j][i] = data_[i][j];
			return std::move(res);
		}

		bool operator==(const Matrix &that) const {
			auto shape = Shape();
			assert(shape == that.Shape());
			for (size_t i = 0; i != shape.first; ++i)
				if (data_[i] != that.data_[i])
					return false;
			return true;
		}

		bool operator!=(const Matrix &that) const {
			return !this->operator==(that);
		}

		Matrix operator-() const {
			auto shape = Shape();
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res.data_[i] = -data_[i];
			return std::move(res);
		}

		size_t Row() const { return data_.size(); }

		size_t Col() const { assert(Row()); return data_[0].size(); }

		std::pair<size_t, size_t> Shape() const { return { Row(), Col() }; }

		size_t Shape(size_t i) const {
			assert(i < 2);
			return i ? Col() : Row();
		}

		template<typename V>
		Matrix Dot(const Matrix<V> &that) const {
			size_t dim = Col();
			assert(dim == that.Row());
			size_t row = Row();
			size_t col = that.Col();
			Matrix<U> res(row, col);
			for (size_t i = 0; i != row; ++i)
				for (size_t k = 0; k != dim; ++k)
					for (size_t j = 0; j != col; ++j)
						res.data_[i][j] += data_[i][k] * that.data_[k][j];
			return std::move(res);
		}

		template<typename V>
		Vector<U> Dot(const Vector<V> &that) const {
			size_t row = that.size();
			assert(Col() == row);
			row = Row();
			Vector<U> res(row);
			for (size_t i = 0; i != row; ++i)
				res.data_[i] = data_[i].Dot(that);
			return std::move(res);
		}
		template<typename V>
		Matrix operator*(const Matrix<V> &that) const {
			auto shape = Shape();
			assert(shape == that.Shape());
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				for (size_t j = 0; j != shape.second; ++j)
					res.data_[i][j] = data_[i][j] * that.data_[i][j];
			return res;
		}
		template<typename V>
		Matrix& operator*=(const Matrix<V> &that) { *this = *this * that; return *this; }
		template<typename V>
		Vector<U> operator*(const Vector<V> &that) const { return Dot(that); }

		template<typename V>
		Matrix operator+(const Matrix<V> &that) const {
			auto shape = Shape();
			assert(shape == that.Shape());
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] + that.data_[i];
			return std::move(res);
		}

		template<typename V>
		Matrix& operator+=(const Matrix<V> &that){
			auto shape = Shape();
			assert(shape == that.Shape());
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] += that.data_[i];
			return *this;
		}
		template<typename V>
		Matrix operator+(const Vector<V> &v) const {
			auto shape = Shape();
			assert(shape.second == v.size());
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] + v;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator+=(const Vector<V> &v) {
			auto shape = Shape();
			assert(shape.second == v.size());
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] += v;
			return *this;
		}
		template<typename V>
		Matrix operator+(const V &val) const {
			auto shape = Shape();
			Matrix<U> res(shape);
			U v = static_cast<U>(val);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] + v;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator+=(const V &val) {
			auto shape = Shape();
			U v = static_cast<U>(val);
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] += v;
			return *this;
		}

		template<typename V>
		Matrix operator-(const Matrix<V> &that) const {
			auto shape = Shape();
			assert(shape == that.Shape());
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] - that.data_[i];
			return std::move(res);
		}
		template<typename V>
		Matrix& operator-=(const Matrix<V> &that){
			auto shape = Shape();
			assert(shape == that.Shape());
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] -= that.data_[i];
			return *this;
		}
		template<typename V>
		Matrix operator-(const V &val) const {
			auto shape = Shape();
			Matrix<U> res(shape);
			U v = static_cast<U>(val);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] - v;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator-=(const V &val) {
			auto shape = Shape();
			U v = static_cast<U>(val);
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] -= v;
			return *this;
		}

		template<typename V>
		Matrix operator*(const V &val) const {
			size_t row = Row();
			Matrix<U> res(Shape());
			for (size_t i = 0; i != row; ++i)
				res.data_[i] = data_[i] * val;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator*=(const V &val) {
			size_t row = Row();
			for (size_t i = 0; i != row; ++i)
				data_[i] *= val;
			return *this;
		}

		template<typename V>
		Matrix operator/(const Vector<V> &v) const {
			auto shape = Shape();
			assert(shape.first == v.size());
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res.data_[i] = data_[i] / v[i];
			return std::move(res);
		}
		template<typename V>
		Matrix& operator/=(const Vector<V> &v) {
			auto shape = Shape();
			assert(shape.first == v.size());
			for (size_t i = 0; i != shape.first; ++i)
				data_[i] /= v[i];
			return *this;
		}
		template<typename V>
		Matrix operator/(const V &val) const {
			assert(val);
			return *this * (1.0 / val);
		}
		template<typename V>
		Matrix& operator/=(const V &val) {
			assert(val);
			*this *= 1.0 / val;
			return *this;
		}

		Vector<U>& operator[](size_t i) {
			// assert(i < Row());
			return data_[i];
		}
		const Vector<U>& operator[](size_t i) const {
			// assert(i < Row());
			return data_[i];
		}

		U Max() const {
			size_t col = Col();
			size_t row = Row();
			assert(row && col);
			U max = data_[0][0];
			std::for_each(data_.begin(), data_.end(), [&max](const Vector<U> &row) {
				U tmp = row.Max();
				max = ((max < tmp) ? tmp : max);
			});
			return max;
		}

		U Sum() const {
			U sum = 0;
			std::for_each(data_.begin(), data_.end(), [&sum](const Vector<U> &row) {
				sum += row.Sum();
			});
			return sum;
		}

		Vector<U> Sum(int axis) const {
			auto shape = Shape();
			if (axis == 0) {
				Vector<U> sum(shape.first);
				for (size_t i = 0; i != shape.first; ++i)
					sum[i] = data_[i].Sum();
				return sum;
			} else {
				Vector<U> sum(shape.second);
				for (size_t i = 0; i != shape.second; ++i)
					for (size_t j = 0; j != shape.first; ++j)
						sum[i] += data_[j][i];
				return sum;
			}
		}

		Matrix<double> Exp() const {
			auto shape = Shape();
			assert(shape.first && shape.second);
			Matrix<double> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i].Exp();
			return res;
		}

		Matrix<double> Log() const {
			auto shape = Shape();
			assert(shape.first && shape.second);
			Matrix<double> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i].Log();
			return res;
		}

		void Push(const Vector<U> &vec) { data_.push_back(vec); }

		static Matrix Randomize(size_t x, size_t y, double scale = 1) {
			static std::default_random_engine generator(time(0));
			static std::normal_distribution<double> distribution(0, 1);
			Matrix<U> res(x, y);
			for (size_t i = 0; i != x; ++i)
				for (size_t j = 0; j != y; ++j)
					res.data_[i][j] = static_cast<U>(distribution(generator) * scale);
			return std::move(res);
		}

		std::string ToString() const {
			std::ostringstream os;
			if (!Row()) return std::string("\n");
			os << Shape().first << " " << Shape().second << std::endl;
			return os.str();
		}

	private:

		void Validate(const Matrix &that) { assert(Shape() == that.Shape()); }

		std::vector<Vector<U>> data_;
};

template<typename T>
Matrix<double> operator/(double x, const Matrix<T> &m) {
	auto shape = m.Shape();
	Matrix<double> res(shape);
	for (size_t i = 0; i != shape.first; ++i)
		for (size_t j = 0; j != shape.second; ++j)
			res[i][j] = x / m[i][j];
	return res;
}

template<typename U, typename T>
Matrix<T> operator-(U u, const Matrix<T> &m) {
	return std::move((-m) + u);
}

} // namespace Elephant

#endif /* _MATRIX_HPP_ */