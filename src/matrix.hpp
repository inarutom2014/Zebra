/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2017-01-03 16:52:41
**/

#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cmath>

#include "vector.hpp"
#include "point.hpp"

namespace Zebra {

class Matrix
{
	public:
		union {
			double m_[4][4];
			double _m[16];
		};

		Matrix() = default;
		Matrix(	double m00, double m10, double m20, double m30, \
            double m01, double m11, double m21, double m31, \
            double m02, double m12, double m22, double m32, \
            double m03, double m13, double m23, double m33) {
			m_[0][0] = m00, m_[1][0] = m10, m_[2][0] = m20, m_[3][0] = m30,
			m_[0][1] = m01; m_[1][1] = m11, m_[2][1] = m21, m_[3][1] = m31,
			m_[0][2] = m02; m_[1][2] = m12, m_[2][2] = m22, m_[3][2] = m32,
			m_[0][3] = m03; m_[1][3] = m13, m_[2][3] = m23, m_[3][3] = m33;
		}

		Matrix operator*(const Matrix &m) const {
			Matrix M;
			for (size_t i = 0; i < 4; ++i)
				for (size_t j = 0; j < 4; ++j)
					M.m_[j][i] =  m_[0][i] * m.m_[j][0] +
												m_[1][i] * m.m_[j][1] +
												m_[2][i] * m.m_[j][2] +
												m_[3][i] * m.m_[j][3];
			return M;
		}

		Matrix& operator*=(const Matrix &m) {
			return *this = operator*(m);
		}

		Vector operator()(const Vector &p) const {
			double x = m_[0][0] * p.x_ + m_[1][0] * p.y_ + m_[2][0] * p.z_ + m_[3][0];
			double y = m_[0][1] * p.x_ + m_[1][1] * p.y_ + m_[2][1] * p.z_ + m_[3][1];
			double z = m_[0][2] * p.x_ + m_[1][2] * p.y_ + m_[2][2] * p.z_ + m_[3][2];
			return Vector(x, y, z);
		}

		Point operator()(const Point &v) const {
			double x = m_[0][0] * v.x_ + m_[1][0] * v.y_ + m_[2][0] * v.z_ + m_[3][0];
			double y = m_[0][1] * v.x_ + m_[1][1] * v.y_ + m_[2][1] * v.z_ + m_[3][1];
			double z = m_[0][2] * v.x_ + m_[1][2] * v.y_ + m_[2][2] * v.z_ + m_[3][2];
			double w = m_[0][3] * v.x_ + m_[1][3] * v.y_ + m_[2][3] * v.z_ + m_[3][3];
			if (w != 1) {
				double inv = 1.0 / w;
				x *= inv;
				y *= inv;
				z *= inv;
			}
			return Point(x, y, z);
		}
};

Matrix Inverse(const Matrix &m)
{
	double inv[16];

	inv[0] = 	m._m[5]  * m._m[10] * m._m[15] -
						m._m[5]  * m._m[11] * m._m[14] -
						m._m[9]  * m._m[6]  * m._m[15] +
						m._m[9]  * m._m[7]  * m._m[14] +
						m._m[13] * m._m[6]  * m._m[11] -
						m._m[13] * m._m[7]  * m._m[10];

	inv[4] = -m._m[4]  * m._m[10] * m._m[15] +
						m._m[4]  * m._m[11] * m._m[14] +
						m._m[8]  * m._m[6]  * m._m[15] -
						m._m[8]  * m._m[7]  * m._m[14] -
						m._m[12] * m._m[6]  * m._m[11] +
						m._m[12] * m._m[7]  * m._m[10];

	inv[8] = 	m._m[4]  * m._m[9]  * m._m[15] -
						m._m[4]  * m._m[11] * m._m[13] -
						m._m[8]  * m._m[5]  * m._m[15] +
						m._m[8]  * m._m[7]  * m._m[13] +
						m._m[12] * m._m[5]  * m._m[11] -
						m._m[12] * m._m[7]  * m._m[9];

	inv[12] = -m._m[4]  * m._m[9]  * m._m[14] +
						 m._m[4]  * m._m[10] * m._m[13] +
						 m._m[8]  * m._m[5]  * m._m[14] -
						 m._m[8]  * m._m[6]  * m._m[13] -
						 m._m[12] * m._m[5]  * m._m[10] +
						 m._m[12] * m._m[6]  * m._m[9];

	inv[1] = -m._m[1]  * m._m[10] * m._m[15] +
						m._m[1]  * m._m[11] * m._m[14] +
						m._m[9]  * m._m[2] *  m._m[15] -
						m._m[9]  * m._m[3] *  m._m[14] -
						m._m[13] * m._m[2] *  m._m[11] +
						m._m[13] * m._m[3] *  m._m[10];

	inv[5] =  m._m[0]  * m._m[10] * m._m[15] -
						m._m[0]  * m._m[11] * m._m[14] -
						m._m[8]  * m._m[2]  * m._m[15] +
						m._m[8]  * m._m[3]  * m._m[14] +
						m._m[12] * m._m[2]  * m._m[11] -
						m._m[12] * m._m[3]  * m._m[10];

	inv[9] = -m._m[0]  * m._m[9]  * m._m[15] +
						m._m[0]  * m._m[11] * m._m[13] +
						m._m[8]  * m._m[1]  * m._m[15] -
						m._m[8]  * m._m[3]  * m._m[13] -
						m._m[12] * m._m[1]  * m._m[11] +
						m._m[12] * m._m[3]  * m._m[9];

	inv[13] = m._m[0]  * m._m[9]  * m._m[14] -
						m._m[0]  * m._m[10] * m._m[13] -
						m._m[8]  * m._m[1]  * m._m[14] +
						m._m[8]  * m._m[2]  * m._m[13] +
						m._m[12] * m._m[1]  * m._m[10] -
						m._m[12] * m._m[2]  * m._m[9];

	inv[2] = m._m[1]  * m._m[6] * m._m[15] -
					 m._m[1]  * m._m[7] * m._m[14] -
					 m._m[5]  * m._m[2] * m._m[15] +
					 m._m[5]  * m._m[3] * m._m[14] +
					 m._m[13] * m._m[2] * m._m[7] -
					 m._m[13] * m._m[3] * m._m[6];

	inv[6] = -m._m[0]  * m._m[6] * m._m[15] +
						m._m[0]  * m._m[7] * m._m[14] +
						m._m[4]  * m._m[2] * m._m[15] -
						m._m[4]  * m._m[3] * m._m[14] -
						m._m[12] * m._m[2] * m._m[7] +
						m._m[12] * m._m[3] * m._m[6];

	inv[10] = m._m[0]  * m._m[5] * m._m[15] -
						m._m[0]  * m._m[7] * m._m[13] -
						m._m[4]  * m._m[1] * m._m[15] +
						m._m[4]  * m._m[3] * m._m[13] +
						m._m[12] * m._m[1] * m._m[7] -
						m._m[12] * m._m[3] * m._m[5];

	inv[14] = -m._m[0]  * m._m[5] * m._m[14] +
						 m._m[0]  * m._m[6] * m._m[13] +
						 m._m[4]  * m._m[1] * m._m[14] -
						 m._m[4]  * m._m[2] * m._m[13] -
						 m._m[12] * m._m[1] * m._m[6] +
						 m._m[12] * m._m[2] * m._m[5];

	inv[3] = -m._m[1] * m._m[6] * m._m[11] +
						m._m[1] * m._m[7] * m._m[10] +
						m._m[5] * m._m[2] * m._m[11] -
						m._m[5] * m._m[3] * m._m[10] -
						m._m[9] * m._m[2] * m._m[7] +
						m._m[9] * m._m[3] * m._m[6];

	inv[7] =  m._m[0] * m._m[6] * m._m[11] -
						m._m[0] * m._m[7] * m._m[10] -
						m._m[4] * m._m[2] * m._m[11] +
						m._m[4] * m._m[3] * m._m[10] +
						m._m[8] * m._m[2] * m._m[7] -
						m._m[8] * m._m[3] * m._m[6];

	inv[11] = -m._m[0] * m._m[5] * m._m[11] +
						 m._m[0] * m._m[7] * m._m[9] +
						 m._m[4] * m._m[1] * m._m[11] -
						 m._m[4] * m._m[3] * m._m[9] -
						 m._m[8] * m._m[1] * m._m[7] +
						 m._m[8] * m._m[3] * m._m[5];

	inv[15] = m._m[0] * m._m[5] * m._m[10] -
	    			m._m[0] * m._m[6] * m._m[9] -
	    			m._m[4] * m._m[1] * m._m[10] +
	    			m._m[4] * m._m[2] * m._m[9] +
	    			m._m[8] * m._m[1] * m._m[6] -
	    			m._m[8] * m._m[2] * m._m[5];

	double det = m._m[0] * inv[0] + m._m[1] * inv[4] + m._m[2] * inv[8] + m._m[3] * inv[12];

	if (det == 0) { return Matrix(1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);
	}

	Matrix ret;
	det = 1.0 / det;
	for (int i = 0; i < 16; ++i)
		ret._m[i] = inv[i] * det;
	return ret;
}

Matrix Transform(const double x, const double y, const double z)
{
	return Matrix(1, 0, 0, x,
								0, 1, 0, y,
								0, 0, 1, z,
								0, 0, 0,  1);
}

Matrix Scale(const double x, const double y, const double z)
{
	return Matrix(x, 0, 0, 0,
								0, y, 0, 0,
								0, 0, z, 0,
								0, 0, 0, 1);
}

Matrix Perspective(const double fov, const double n, const double f)
{
	double n_f = n - f;
	return Matrix(fov, 	0,  0,        0,
								0, 	fov,  0,        0,
								0,  0, 	f/n_f,     -1,
								0, 	0, 	(f*n)/n_f,  0);
}

} // namespace Zebra

#endif /* _MATRIX_HPP_ */