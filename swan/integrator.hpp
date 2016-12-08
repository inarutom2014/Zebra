/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-05 22:36:27
**/

#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_

#include <random>
#include <ctime>
#include <sstream>
#include <fstream>

#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"

namespace Swan {

class Integrator
{
	public:
		Integrator(int samples, const Point2<int> &pixel_bound, const Scene &scene)
		:samples_(samples), pixel_bound_(pixel_bound), scene_(scene),
     pixels_(new Vector[pixel_bound_.x_ * pixel_bound_.y_]),
     generator_(time(0)), distribution_(0, 1) { }

		void Render() {
			Spectrum L;
			for (int x = 0; x != pixel_bound_.x_; ++x) {
				for (int y = 0; y != pixel_bound_.y_; ++y, L = Spectrum()) {
					for (int n = 0; n != samples_; ++n) {
						double dx = 2 * distribution(generator) - 1;
						double dy = 2 * distribution(generator) - 1;
						Ray ray(Point(), Vector((dx + x) / pixel_bound_.x_,
                                    (dy + y) / pixel_bound_.y_,
                                    -1));
						L += Li(ray);
					}
					pixels_[x * pixel_bound_.y_ + y] = L / samples_;
				}
			}
			WriteImage();
		}

		virtual Spectrum Li(Ray ray) const = 0;

		virtual ~Integrator() { delete [] pixels_; }

	protected:

		void WriteImage() {
			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
			std::string file(os.str());
			file += ".ppm";
			std::cerr << "save to: " << file << std::endl;

			std::ofstream out(file, std::ios::out | std::ios::binary);
			if (!out.is_open()) { std::cerr << "ppm格式图片保存失败 :(\n"; return ; }
			out << "P3\n" << pixel_bound_.x_ << " " << pixel_bound_.y_ << "\n255\n";
			for (int i = 0; i < pixel_bound_.x_ * pixel_bound_.y_; ++i) {
				int r = static_cast<int>(std::pow(std::min(pixels_[i].x_, 1.0), 1/2.2) * 255 + 0.5);
				int g = static_cast<int>(std::pow(std::min(pixels_[i].y_, 1.0), 1/2.2) * 255 + 0.5);
				int b = static_cast<int>(std::pow(std::min(pixels_[i].z_, 1.0), 1/2.2) * 255 + 0.5);
				out << r << " " << g << " " << b << " ";
			}
			out.close();
		}

		const int         samples_;
		const Point2<int> pixel_bound_;
		const Scene       scene_;
		Vector           *pixels_;

		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;
};

} // namespace Swan

#endif /* _INTEGRATOR_HPP_ */