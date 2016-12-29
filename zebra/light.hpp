/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 23:59:51
**/

#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "vertex.hpp"
#include "object.hpp"
#include "sampling.hpp"
#include "ray.hpp"
#include "parameter.hpp"

namespace Zebra {

class Light
{
	public:
		Light(const Spectrum &intensity):intensity_(intensity) { }

		virtual Spectrum SampleLi(const Point &position, const Point2 &u,
			Vector &wi, double &dis, double &pdf) const = 0;

		virtual Spectrum SampleLe(Ray &ray, const Point2 &u,
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

		Spectrum SampleLi(const Point &position, const Point2 &u, Vector &wi, double &dis,
			double &pdf) const {
			Vector dir(position_ - position);
			double tmp = dir.Length2();
			dis = std::sqrt(tmp);
			wi  = dir / dis;
			pdf = 1;
			return intensity_ / tmp;
		}

		Spectrum SampleLe(Ray &ray, const Point2 &u, double &pdf_pos, double &pdf_dir) const {
			ray = Ray(position_, UniformSampleSphere(u));
			pdf_pos = 1.0;
			pdf_dir = INV_PI * 0.25;
			return intensity_;
		}

	private:
		const Point position_;
};

class AreaLight : public Light
{
	public:
		AreaLight(const Object *object, const Spectrum &intensity)
		:Light(intensity), object_(object) { }

		Spectrum SampleLi(const Point &position, const Point2 &u, Vector &wi, double &dis,
			double &pdf) const {
			Vertex v = object_->Sample(u);
			Vector dir(v.position_ - position);
			dis = dir.Length();
			wi  = dir / dis;
			pdf = object_->Pdf(v);
			return L(v, -wi);
		}

		Spectrum SampleLe(Ray &ray, const Point2 &u, double &pdf_pos, double &pdf_dir) const {
			Vertex v = object_->Sample(u);
			pdf_pos = object_->Pdf(v);
			Vector w = CosineWeightedHemisphere(u);
			pdf_dir = CosineHemispherePdf(CosTheta(w));

			Vector x, y, z(v.normal_);
			MakeCoordinateSystem(z, x, y);

			w = x * w.x_ + y * w.y_ + z * w.z_;

			ray = Ray(v.position_, w);
			return L(v, w);
		}

		Spectrum L(const Vertex &v, const Vector &w) const {
			return Dot(v.normal_, w) > 0 ? intensity_ : Spectrum();
		}

		~AreaLight() { delete object_; }

		const Object *object_;
};

Light* NewPointLight(Parameter &param)
{
	Point  position  = param.FindPosition();
	Vector intensity = param.FindVector();
	return new PointLight(position, intensity);
}

AreaLight* NewAreaLight(Parameter &param)
{
	std::string o(param.FindString());
	assert(o == "Sphere");
	const Object *object = NewSphere(param);
	Vector intensity = param.FindVector();
	return new AreaLight(object, intensity);
}

} // namespace Zebra

#endif /* _LIGHT_HPP_ */