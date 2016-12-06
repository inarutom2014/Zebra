/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-05 23:35:11
**/

#ifndef _PATH_TRACER_HPP_
#define _PATH_TRACER_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"
#include "light.hpp"
#include "integrator.hpp"
#include "parser.hpp"

namespace Swan {

class PathTracer : public Integrator
{
	public:
		PathTracer(int samples, const Point2<int> &pixel_bound, int max_depth)
		:Integrator(samples, pixel_bound), max_depth_(max_depth) { }

		void Trace() const override {

		}

	private:
		int max_depth_;
};

} // namespace Swan

#endif /* _PATH_TRACER_HPP_ */