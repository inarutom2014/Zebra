/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 21:31:20
**/

#include <unistd.h>

#include "parser.hpp"
#include "path_tracer.hpp"

int main(int argc, char **argv)
{
	using namespace Swan;
	int samples = 0;
	if (argc > 1) samples = atoi(argv[1]);
	int depth = 0;
	if (argc > 2) depth = atoi(argv[2]);
	if (!samples) samples = 1;
	if (!depth) depth = 0;

	Parser parser("scene.Swan");
	Integrator *path_tracer = new PathTracer(samples, parser.GetScene(), depth);
	std::string image = path_tracer->Render();
	execlp("display", "display", image.c_str(), NULL);
	delete path_tracer;

	return 0;
}
