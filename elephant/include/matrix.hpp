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
#include <iostream>
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

		size_t Row() const { return data_.size(); }

		size_t Col() const { assert(Row()); return data_[0].size(); }

		std::pair<size_t, size_t> Shape() const {
			return { Row(), Col() };
		}

		size_t Shape(size_t i) const {
			assert(i < 2);
			return i ? Col() : Row();
		}

		void SetRow(size_t i, const Vector<U> &vector) {
			assert(i < Row());
			data_[i] = vector;
		}

		void SetCol(size_t i, const Vector<U> &vector) {
			assert(i < Col());
			size_t row = Row();
			assert(row == vector.size());
			for (size_t j = 0; j != row; ++j)
				data_[j][i] = vector.data_[j];
		}

		template<typename V>
		Matrix Dot(const Matrix<V> &that) const {
			assert(Col() == that.Row());
			size_t row = Row();
			size_t col = that.Col();
			size_t dim = Col();
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
		Matrix operator+(const V &v) const {
			auto shape = Shape();
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] + v;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator+=(const V &v) {
			auto shape = Shape();
			Matrix<U> res(shape);
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
		Matrix operator-(const V &v) const {
			auto shape = Shape();
			Matrix<U> res(shape);
			for (size_t i = 0; i != shape.first; ++i)
				res[i] = data_[i] - v;
			return std::move(res);
		}
		template<typename V>
		Matrix& operator-=(const V &v) {
			auto shape = Shape();
			Matrix<U> res(shape);
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
			assert(i < Row());
			return data_[i];
		}
		const Vector<U>& operator[](size_t i) const {
			assert(i < Row());
			return data_[i];
		}

		Vector<U> Row(size_t r) const {
			assert(r < Row());
			return std::move(data_[r]);
		}

		Vector<U> Col(size_t c) const {
			assert(c < Col());
			size_t row = Row();
			Vector<U> res(row);
			for (size_t i = 0; i != row; ++i)
				res.data_[i] = data_[i][c];
			return std::move(res);
		}

		U Max() const {
			size_t col = Col();
			size_t row = Row();
			assert(row && col);
			U max = data_[0][0];
			std::for_each(data_.begin(), data_.end(), [&max](const Vector<U> &row) {
				std::for_each(row.begin(), row.end(), [&max](const U &val) {
					if (val > max)
						max = val;
				});
			});
			return max;
		}

		U Sum() const {
			size_t col = Col();
			size_t row = Row();
			assert(row && col);
			U sum = 0;
			std::for_each(data_.begin(), data_.end(), [&sum](const Vector<U> &row) {
				std::for_each(row.rbegin(), row.rend(), [&sum](const U &val) {
					sum += val;
				});
			});
			return sum;
		}

		void Push(const Vector<U> &vec) { data_.push_back(vec); }

		static Matrix Randomize(size_t x, size_t y, double scale) {
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

} // namespace Elephant

#endif /* _MATRIX_HPP_ */