/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 22:36:27
**/

#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_

#include <random>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>

#include "vector.hpp"
#include "camera.hpp"

namespace Zebra {

class Integrator
{
	public:
		Integrator(int samples, const Scene &scene)
		:samples_(samples), camera_(Camera()), scene_(scene),
		 pixels_(new Vector[camera_.resolution_.x_ * camera_.resolution_.y_]),
		 generator_(time(0)), distribution_(0, 1) { }

		virtual std::string Render() = 0;

		virtual ~Integrator() { delete [] pixels_; }

	protected:

		std::string WriteImage() {
			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
			std::string file("../image/" + os.str());
			file += ".ppm";
			std::cerr << "save to: " << os.str() << ".ppm\n";

			std::ofstream out(file, std::ios::out | std::ios::binary);
			if (!out.is_open()) { std::cerr << "ppm格式图片保存失败 :(\n"; return ""; }
			int x = camera_.resolution_.x_, y = camera_.resolution_.y_;
			out << "P3\n" << x << " " << y << "\n255\n";
			for (int i = 0; i < x * y; ++i) {
				int r = static_cast<int>(std::min(pixels_[i].x_, 1.0) * 255);
				int g = static_cast<int>(std::min(pixels_[i].y_, 1.0) * 255);
				int b = static_cast<int>(std::min(pixels_[i].z_, 1.0) * 255);
				out << r << " " << g << " " << b << " ";
			}
			out.close();
			return file;
		}

		const int         samples_;
		const Camera      camera_;
		const Scene       scene_;
		Vector           *pixels_;

		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;
};

} // namespace Zebra

#endif /* _INTEGRATOR_HPP_ */