/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-29 21:09:33
**/

#ifndef _BI_DIRECTIONAL_PATH_TRACER_HPP_
#define _BI_DIRECTIONAL_PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "integrator.hpp"

namespace Zebra {

class Vertex : public Isect
{
	public:
		static enum { Camera, Light, Surface } Type;

	private:
		Type     type_;
		union {
			Intersection         si_;
			EndPointIntersection ei_;
		};
		Spectrum weight_;
		bool     delta_;
		double   pdf_forward_;
		double   pdf_backward_;
};

class BiDirectionalPathTracer : public Integrator
{
	public:
		BiDirectionalPathTracer(int iterations, const Scene &scene)
		:Integrator(iterations, scene) { }

		std::string Render() {

		}

	private:

};

} // namespace Zebra

#endif /* _BI_DIRECTIONAL_PATH_TRACER_HPP_ */