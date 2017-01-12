/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 21:31:20
**/

#include <unistd.h>
#include <string>
#include <cstring>

#include "parser.hpp"
#include "path_tracer.hpp"
#include "light_tracer.hpp"
#include "bi_path_tracer.hpp"
#include "pure_path_tracer.hpp"

int main(int argc, char **argv)
{
	if (argc < 4) return 0;
	using namespace Zebra;
	int samples = 0;
	char scene[32] = {0};
	if (argc > 2) strcpy(scene, argv[1]);
	else strcpy(scene, "box");
	strcat(scene, ".Zebra");
	if (argc > 3) samples = atoi(argv[3]);
	if (samples <= 0) samples = 1;

	Parser parser(scene);
	Integrator *integrator = nullptr;
	if (!strcmp("lt", argv[2]))
		integrator = new LightTracer(samples, parser.GetScene());
	else if (!strcmp("ppt", argv[2]))
		integrator = new PurePathTracer(samples, parser.GetScene());
	else
		integrator = new PathTracer(samples, parser.GetScene());
	std::string image = integrator->Render();
	execlp("subl", "subl", image.c_str(), nullptr);

	delete integrator;
	return 0;
}
