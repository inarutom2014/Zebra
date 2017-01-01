/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2017-01-01 09:18:34
**/

#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include <fstream>
#include <cassert>

#include "vector.h"
#include "rng.h"
#include "camera.h"

namespace Zebra {

class Integrator
{
	public:
		Integrator():pixels_(new Spectrum[camera_.x_ * camera_.y_]) { }

		virtual std::string Render(const std::vector<Object *> &scene) = 0;

		virtual ~Integrator() { }

		std::string WritePPM() const {
			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
			std::string file("../image/" + os.str());
			file += ".ppm";
			fprintf(stderr, "save to: %s.ppm\n", os.str().c_str());

			std::ofstream out(file, std::ios::out | std::ios::binary);
			assert(out.is_open());
			out << "P3\n" << camera_.x_ << " " << camera_.y_ << "\n255\n";
			for (int i = 0; i < camera_.x_ * camera_.y_; ++i) {
				int r = static_cast<int>(std::min(pixels_[i].x_, 1.0) * 255);
				int g = static_cast<int>(std::min(pixels_[i].y_, 1.0) * 255);
				int b = static_cast<int>(std::min(pixels_[i].z_, 1.0) * 255);
				out << r << " " << g << " " << b << " ";
			}
			out.close();
			return file;
		}

	protected:
		RNG     rng_;
		Camera  camera_;
		Vector *pixels_;
};

} // namespace Zebra

#endif /* _INTEGRATOR_H_ */