/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 23:59:51
**/

#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "constant.hpp"
#include "point.hpp"
#include "vector.hpp"
#include "ray.hpp"
#include "parameter.hpp"

namespace Zebra {

static inline Vector UniformSphere(const Point2<double> &u)
{
	double z = 1 - 2 * u.x_;
	double r = std::sqrt(std::max(0.0, 1 - z * z));
	double phi = 2 * PI * u.y_;
	return Normalize(Vector(r * std::cos(phi), r * std::sin(phi), z));
}

class Light
{
	public:
		Light(const Spectrum &intensity):intensity_(intensity) { }

		virtual Spectrum SampleLi(const Point &position, Vector &wi, double &dis, double &pdf)
      const = 0;

		virtual Spectrum SampleLe(Ray &ray, const Point2<double> &u,
      double &pdf_pos, double &pdf_dir) const = 0;

		virtual ~Light() { }

	protected:
		const Spectrum intensity_;
};

class PointLight : public Light
{
	public:
		PointLight(const Point &position, const Spectrum &intensity)
		:Light(intensity), position_(position) { }

		Spectrum SampleLi(const Point &position, Vector &wi, double &dis, double &pdf) const {
			Vector dir(position_ - position);
			double tmp = dir.Length2();
			dis = std::sqrt(tmp);
			wi  = dir / dis;
			pdf = 1;
			return intensity_ / tmp;
		}

		Spectrum SampleLe(Ray &ray, const Point2<double> &u,
			double &pdf_pos, double &pdf_dir) const {
			ray = Ray(position_, UniformSphere(u));
			pdf_pos = 1.0;
			pdf_dir = INV_PI * 0.25;
			return intensity_;
		}

	private:
		const Point position_;
};

Light* NewPointLight(Parameter &param)
{
	Point  position  = param.FindPosition();
	Vector intensity = param.FindVector();
	return new PointLight(position, intensity);
}

} // namespace Zebra

#endif /* _LIGHT_HPP_ */