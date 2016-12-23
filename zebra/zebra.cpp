/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 21:31:20
**/

// #include <unistd.h>
#include <string>
#include <cstring>

#include "parser.hpp"
#include "path_tracer.hpp"

int main(int argc, char **argv)
{
	using namespace Zebra;
	int samples = 0;
	char scene[32] = {0};
	if (argc > 2) strcpy(scene, argv[1]);
	else strcpy(scene, "planet");
	printf("scene: %s\n", scene);
	strcat(scene, ".Zebra");
	if (argc > 2) samples = atoi(argv[2]);
	if (samples <= 0) samples = 1;

	Parser parser(scene);
	Integrator *path_tracer = new PathTracer(samples, parser.GetScene());
	std::string image = path_tracer->Render();
	delete path_tracer;
	// execlp("display", "display", image.c_str(), nullptr);

	return 0;
}
