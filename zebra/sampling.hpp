/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-19 09:43:41
**/

#ifndef _SAMPLING_HPP_
#define _SAMPLING_HPP_

#include "vector.hpp"
#include "point.hpp"

namespace Zebra {

inline void MakeCoordinateSystem(const Vector &n, Vector &n1, Vector &n2)
{
	if (std::fabs(n.x_) > std::fabs(n.y_))
		n1 = Normalize(Cross(Vector(0, 1, 0), n));
	else
		n1 = Normalize(Cross(Vector(1, 0, 0), n));
	n2 = Cross(n, n1);
}

inline Vector UniformSampleSphere(const Point2 &u)
{
	double z = 1 - 2 * u.x_;
	double r = std::sqrt(std::max(0.0, 1.0 - z * z));
	double phi = 2 * PI * u.y_;
	return Vector(r * std::cos(phi), r * std::sin(phi), z);
}

inline Vector CosineWeightedHemisphere(const Point2 &u)
{
	double theta = std::asin(std::sqrt(u.x_));
	double phi = 2 * PI * u.y_;

	double sini = std::sin(theta);
	double cosi = std::cos(theta);

	double xs = sini * std::cos(phi);
	double ys = sini * std::sin(phi);
	double zs = cosi;

	return Normalize(Vector(xs, ys, zs));
}

inline double CosineHemispherePdf(double theta) {
	return theta * INV_PI;
}

inline double UniformConePdf(double cos_theta) {
    return 1.0 / (2 * PI * (1 - cos_theta));
}

} // namespace Zebra

#endif /* _SAMPLING_HPP_ */