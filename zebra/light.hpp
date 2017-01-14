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
#include "object.hpp"
#include "sampling.hpp"
#include "ray.hpp"
#include "parameter.hpp"
#include "intersection.hpp"

namespace Zebra {

class Light
{
	public:
		Light(const Spectrum &intensity):intensity_(intensity) { }

		virtual bool IsFinite() const = 0;
		virtual bool IsDelta() const = 0;

		virtual Spectrum SampleLi(const Point &position, const Point2 &u,
			Vector &wi, double &dis, double &pdf) const = 0;

		virtual Spectrum SampleLe(Ray &ray, Vector &normal, const Point2 &u,
			double &pdf_pos, double &pdf_dir) const = 0;

		virtual void PdfLe(const Ray &ray, const Vector &normal, double &pdf_pos, double &pdf_dir)
			const = 0;

		virtual ~Light() { }

	protected:
		const Spectrum intensity_;
};

class PointLight : public Light
{
	public:
		PointLight(const Point &position, const Spectrum &intensity)
		:Light(intensity), position_(position) { }

		bool IsFinite() const { return true; }
		bool IsDelta() const { return true; }

		Spectrum SampleLi(const Point &position, const Point2 &u, Vector &wi, double &dis,
			double &pdf) const {
			Vector dir(position_ - position);
			double tmp = dir.Length2();
			dis = std::sqrt(tmp);
			wi  = dir / dis;
			pdf = 1;
			return intensity_ / tmp;
		}

		Spectrum SampleLe(Ray &ray, Vector &normal, const Point2 &u,
			double &pdf_pos, double &pdf_dir) const {
			ray = Ray(position_, UniformSampleSphere(u));
			normal = ray.direction_;
			pdf_pos = 1.0;
			pdf_dir = INV_PI * 0.25;
			return intensity_;
		}

		void PdfLe(const Ray &ray, const Vector &normal, double &pdf_pos, double &pdf_dir) const {
			pdf_pos = 0;
			pdf_dir = 1.0 / (PI * 4);
		}

	private:
		const Point position_;
};

class AreaLight : public Light
{
	public:
		AreaLight(const Object *object, const Spectrum &intensity)
		:Light(intensity), object_(object) { }

		bool IsFinite() const { return true; }
		bool IsDelta() const { return false; }

		Spectrum SampleLi(const Point &position, const Point2 &u, Vector &wi, double &dis,
			double &pdf) const {
			object_->SampleD(position, u, wi, pdf, dis);
			return intensity_;
		}

		Spectrum SampleLe(Ray &ray, Vector &normal, const Point2 &u,
			double &pdf_pos, double &pdf_dir) const {
			Point pos;
			object_->SampleL(u, pos, normal, pdf_pos);
			Vector tmp = CosineWeightedHemisphere(u);
			pdf_dir = CosineHemispherePdf(CosTheta(tmp));

			Vector x, y, z(normal);
			MakeCoordinateSystem(z, x, y);

			Vector dir = x * tmp.x_ + y * tmp.y_ + z * tmp.z_;

			ray = Ray(pos, dir);
			return intensity_;
		}

		void PdfLe(const Ray &ray, const Vector &normal, double &pdf_pos, double &pdf_dir) const {
			pdf_pos = object_->Pdf(ray.origin_);
			pdf_dir = CosineHemispherePdf(Dot(normal, ray.direction_));
		}

		Spectrum L() const { return intensity_; }

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