/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 21:31:20
**/

#include <unistd.h>

#include "parser.hpp"
#include "path_tracer.hpp"
#include "light_tracer.hpp"

int main(int argc, char **argv)
{
	using namespace Zebra;
	int samples = 0;
	if (argc > 1) samples = atoi(argv[1]);
	if (!samples) samples = 1;

	Parser parser("scene.Zebra");
#ifdef LT
	Integrator *light_tracer = new LightTracer(samples, parser.GetScene());
	std::string image = light_tracer->Render();
	delete light_tracer;
#else
	Integrator *path_tracer = new PathTracer(samples, parser.GetScene());
	std::string image = path_tracer->Render();
	delete path_tracer;
#endif
	execlp("display", "display", image.c_str(), NULL);

	return 0;
}
