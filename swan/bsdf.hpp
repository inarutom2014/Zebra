/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 14:50:32
**/

#ifndef _BSDF_HPP_
#define _BSDF_HPP_

#include <random>
#include <ctime>

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "parameter.hpp"

namespace Swan {

static std::default_random_engine generator(time(0));
static std::uniform_real_distribution<double> distribution(0, 1);

static inline double CosTheta(const Vector &wi)
{
	return wi.z_;
}

static inline Vector CosineWeightedHemisphere()
{
	double u1 = distribution(generator);
	double u2 = distribution(generator);

	double theta = std::asin(std::sqrt(u1));
	double phi = 2 * PI * u2;

	double sini = std::sin(theta);
	double cosi = std::cos(theta);

	double xs = sini * std::cos(phi);
	double ys = sini * std::sin(phi);
	double zs = cosi;

	return Normalize(Vector(xs, ys, zs));
}

class BSDF
{
	public:
		virtual Spectrum F(const Vector &wo, const Vector &wi) const = 0;

		virtual Spectrum SampleF(const Vector &wo, Vector &wi, double &pdf) const = 0;

		virtual bool IsDelta() const { return false; }

		virtual ~BSDF() { }
};

class DiffuseBSDF : public BSDF
{
	public:
		DiffuseBSDF(const Spectrum &albedo):albedo_(albedo) { }

		virtual Spectrum F(const Vector &wo, const Vector &wi) const override {
			return albedo_ * INV_PI;
		}

		virtual Spectrum SampleF(const Vector &wo, Vector &wi, double &pdf) const override {
			wi  = CosineWeightedHemisphere();
			pdf = CosTheta(wi) * INV_PI;
			return F(wo, wi);
		}

	private:
		const Spectrum albedo_;
};

class ReflectBSDF : public BSDF
{
	public:
		ReflectBSDF(const Spectrum &reflectance):reflectance_(reflectance) { }

		Spectrum F(const Vector &wo, const Vector &wi) const override {
			return Spectrum();
		}

		Spectrum SampleF(const Vector &wo, Vector &wi, double &pdf) const override {
			wi = Vector(-wo.x_, -wo.y_, wo.z_);
			pdf = 1.0;
			return reflectance_ * (1.0 / CosTheta(wo));
		}

		bool IsDelta() const override { return true; }

	private:
		const Spectrum reflectance_;
};

// class RefractBSDF : public BSDF
// {
// 	public:
// 		RefractBSDF(const Spectrum &transmittance, double ior)
// 		:transmittance_(transmittance), ior_(ior) { }

// 		virtual Spectrum F(const Vector &wo, const Vector &wi) override {
// 			return Spectrum();
// 		}

// 		virtual Spectrum SampleF(const Vector &wo, Vector &wi, double &pdf) override {
// 			return Spectrum();
// 		}

// 		bool IsDelta() const override { return true; }

// 	private:
//   	const Spectrum transmittance_;
// 		const double   ior_;
// };

BSDF* NewDiffuseBSDF(Parameter &param)
{
	Spectrum spectrum = param.FindVector();
	return new DiffuseBSDF(spectrum);
}

BSDF* NewReflectBSDF(Parameter &param)
{
	Spectrum spectrum = param.FindVector();
	return new ReflectBSDF(spectrum);
}

} // namespace Swan

#endif /* _BSDF_HPP_ */