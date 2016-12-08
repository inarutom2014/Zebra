/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 21:31:20
**/

#include "parser.hpp"
#include "path_tracer.hpp"

int main()
{
	using namespace Swan;

	Parser parser("");
	Integrator *path_tracer = new PathTracer(4, Point2<int>(512, 512), parser.GetScene(), 5);
	path_tracer->Render();
	delete path_tracer;

	return 0;
}
