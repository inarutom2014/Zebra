/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 14:50:32
**/

#ifndef _BSDF_HPP_
#define _BSDF_HPP_

#include <random>
#include <ctime>

#include "constant.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "sampling.hpp"
#include "parameter.hpp"

namespace Zebra {

static inline double CosTheta(const Vector &wi)
{
	return wi.z_;
}

static inline double AbsCosTheta(const Vector &wi)
{
	return std::abs(wi.z_);
}

class BSDF
{
	public:
		BSDF(const Spectrum &r):r_(r) { }

		virtual Spectrum F(const Vector &wo, const Vector &wi) const = 0;

		virtual Spectrum SampleF(const Vector &wo, const Point2 &u, Vector &wi, double &pdf)
		const = 0;

		virtual bool IsDelta() const { return false; }

		virtual double Pdf() const { return 0.0; }

		virtual ~BSDF() { }

	protected:
		const Spectrum r_;
};

class DiffuseBSDF : public BSDF
{
	public:
		DiffuseBSDF(const Spectrum &r):BSDF(r) { }

		Spectrum F(const Vector &wo, const Vector &wi) const override {
			return r_ * INV_PI;
		}

		Spectrum SampleF(const Vector &wo, const Point2 &u, Vector &wi, double &pdf) const {
			wi  = CosineWeightedHemisphere(u);
			pdf = CosineHemispherePdf(CosTheta(wi));
			return F(wo, wi);
		}

		double Pdf(const Vector &wo, const Vector &wi) const { return AbsCosTheta(wi) * INV_PI; }
};

class ReflectBSDF : public BSDF
{
	public:
		ReflectBSDF(const Spectrum &r):BSDF(r) { }

		Spectrum F(const Vector &wo, const Vector &wi) const {
			return Spectrum();
		}

		Spectrum SampleF(const Vector &wo, const Point2 &u, Vector &wi, double &pdf) const {
			wi = Vector(-wo.x_, -wo.y_, wo.z_);
			pdf = 1.0;
			return r_ * (1.0 / CosTheta(wi));
		}

		bool IsDelta() const { return true; }
};

class RefractBSDF : public BSDF
{
	public:
		RefractBSDF(const Spectrum &r, double etai, double etat)
		:BSDF(r), etai_(etai), etat_(etat) { }

		Spectrum F(const Vector &wo, const Vector &wi) const override {
			return Spectrum();
		}

		Spectrum SampleF(const Vector &wo, const Point2 &u, Vector &wi, double &pdf) const {
			bool entering = CosTheta(wo) < 0;
			double etai = entering ? etai_ : etat_;
			double etat = entering ? etat_ : etai_;

			double eta = etai / etat;

			double cosi = AbsCosTheta(wo);

			double sini = std::max(0.0, 1 - cosi * cosi);
			double sint = eta * eta * sini;

			if (sint >= 1) return Spectrum(0);

			double cost = std::sqrt(1 - sint);
			double term1 = etai * cost;
			double term2 = etat * cosi;
			double term3 = etai * cosi;
			double term4 = etat * cost;

			double parl = (term2 - term1) / (term2 + term1);
			double perp = (term3 - term4) / (term3 + term4);
			double re = (parl * parl + perp * perp) * 0.5;
			wi = Vector(-wo.x_ * eta, -wo.y_ * eta, entering ? cost : -cost);
			pdf = 1 - re;
			return r_ * ((1 - re) / AbsCosTheta(wi));
		}

		bool IsDelta() const override { return true; }

	private:
		const double etai_;
		const double etat_;
};

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

BSDF* NewRefractBSDF(Parameter &param)
{
	Spectrum spectrum = param.FindVector();
	double etai = param.FindDouble();
	double etat = param.FindDouble();
	return new RefractBSDF(spectrum, etai, etat);
}

} // namespace Zebra

#endif /* _BSDF_HPP_ */