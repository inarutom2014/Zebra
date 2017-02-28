/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Zebra
 *    > Created Time:  2017-01-01 00:03:01
**/

#include <unistd.h>
#include <string>
#include <cstring>

#include "parser.h"
#include "path_tracer.h"

int main(int argc, char **argv)
{
	using namespace Zebra;
	char scene[32] = {0};
	if (argc > 2)
		strcpy(scene, argv[1]);
	else
		strcpy(scene, "box");
	strcat(scene, ".Zebra");
	int samples = 1;
	if (argc > 2)
		samples = atoi(argv[2]);
	if (samples < 1)
		samples = 1;

	Parser parser(scene);
	Integrator *integrator = new PathTracer(samples);
	std::string image = integrator->Render(parser.Scene());
	execlp("subl", "subl", image.c_str(), nullptr);

	delete integrator;
	return 0;
}
