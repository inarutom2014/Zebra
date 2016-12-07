/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 23:59:51
**/

#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "point.hpp"
#include "vector.hpp"
#include "ray.hpp"
#include "parameter.hpp"

namespace Swan {

static inline Vector UniformSphere(const Point2<double> &u) {
	double z = 1 - 2 * u.x_;
	double r = std::sqrt(std::max(0.0, 1 - z * z));
	double phi = 2 * PI * u.y_;
	return Normalize(Vector(r * std::cos(phi), r * std::sin(phi), z));
}

class Light
{
	public:
		Light() { }

		virtual Spectrum SampleLi(const Point &position, Vector &wi, double &pdf) const = 0;

		virtual Spectrum SampleLe(Vector &wi, const Point2<double> &u,
			double &pdf_pos, double &pdf_dir) const = 0;

		virtual ~Light() { }
};

class DirectionalLight : public Light
{
	public:
		DirectionalLight(const Vector &direction, const Spectrum &intensity)
		:direction_(-Normalize(direction)), intensity_(intensity) { }

		Spectrum SampleLi(const Point &position, Vector &wi, double &pdf) const override {
			wi  = direction_;
			pdf = 1.0;
			return intensity_;
		}

		Spectrum SampleLe(Vector &wi, const Point2<double> &u,
			double &pdf_pos, double &pdf_dir) const override {
			wi = direction_;
			pdf_pos = 1.0;
			pdf_dir = 1.0;
			return intensity_;
		}

	private:
		const Vector   direction_;
		const Spectrum intensity_;
};

class PointLight : public Light
{
	public:
		PointLight(const Point &position, const Spectrum &intensity)
		:position_(position), intensity_(intensity) { }

		Spectrum SampleLi(const Point &position, Vector &wi, double &pdf) const override {
			wi  = Normalize(position_ - position);
			pdf = 1.0;
			return intensity_;
		}

		Spectrum SampleLe(Vector &wi, const Point2<double> &u,
			double &pdf_pos, double &pdf_dir) const override {
			wi = UniformSphere(u);
			pdf_pos = 1.0;
			pdf_dir = INV_PI * 0.25;
			return intensity_;
		}

	private:
		const Point    position_;
		const Spectrum intensity_;
};

Light* NewPointLight(Parameter &param)
{
	Point  position  = param.FindPosition();
	Vector intensity = param.FindVector();
	return new PointLight(position, intensity);
}

Light* NewDirectionalLight(Parameter &param)
{
	Vector direction = param.FindVector();
	Vector intensity = param.FindVector();
	return new DirectionalLight(direction, intensity);
}

} // namespace Swan

#endif /* _LIGHT_HPP_ */