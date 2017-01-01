/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2017-01-01 13:52:07
**/

#ifndef _BI_PATH_TRACER
#define _BI_PATH_TRACER

#include <chrono>

#include "vector.h"
#include "ray.h"
#include "object.h"
#include "interaction.h"
#include "integrator.h"

namespace Zebra {

class BiPathTracer
{
	public:
		BiPathTracer(int samples):samples_(samples) { }

		std::string Render(const std::vector<Object *> &scene) {

		}

	private:
		const int samples_;
};

} // namespace Zebra

#endif /* _BI_PATH_TRACER_ */