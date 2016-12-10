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
#include <string>
#include <chrono>
#include <iomanip>

#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"

namespace Zebra {

class Integrator
{
	public:
		Integrator(int samples, const Scene &scene)
		:scene_(scene), generator_(time(0)), distribution_(0, 1),
		 samples_(samples), pixel_bound_(Point2<int>(512, 512)),
		 pixels_(new Vector[pixel_bound_.x_ * pixel_bound_.y_]) { }

		std::string Render() {
			Spectrum L;
			double cx = (double)pixel_bound_.x_ / pixel_bound_.y_;
			double cy = 1;
			auto beg = std::chrono::high_resolution_clock::now();
			#pragma omp parallel for schedule(dynamic, 1) private(L)
			for (int x = 0; x < pixel_bound_.x_; ++x) {
				std::cout << "\rprogress: " << (100 * x / (pixel_bound_.x_-1)) << " %";
				for (int y = 0; y < pixel_bound_.y_; ++y, L = Spectrum()) {
					for (int n = 0; n < samples_; ++n) {
						double dx = distribution(generator) - 0.5;
						double dy = distribution(generator) - 0.5;
						Ray ray(Point(), Normalize(Vector(cx * (((dx + x) / pixel_bound_.x_) - 0.5),
                                              cy * (0.5 - ((dy + y) / pixel_bound_.y_)),
                                              -1)));
						L += Li(ray);
					}
					pixels_[y * pixel_bound_.x_ + x] = L / samples_;
				}
			}
			auto end  = std::chrono::high_resolution_clock::now();
			auto t = std::chrono::duration<double, std::ratio<1>>(end - beg).count();
			std::cerr << "\ntime: " << std::setw(8) << t << "  s\n";
			return WriteImage();
		}

		virtual Spectrum Li(Ray ray) const = 0;

		virtual ~Integrator() { delete [] pixels_; }

	protected:

		const Scene       scene_;

		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;

	private:
		const int         samples_;
		const Point2<int> pixel_bound_;
		Vector           *pixels_;

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
			out << "P3\n" << pixel_bound_.x_ << " " << pixel_bound_.y_ << "\n255\n";
			for (int i = 0; i < pixel_bound_.x_ * pixel_bound_.y_; ++i) {
				int r = static_cast<int>(std::pow(std::min(pixels_[i].x_, 1.0), 1/2.2) * 255 + 0.5);
				int g = static_cast<int>(std::pow(std::min(pixels_[i].y_, 1.0), 1/2.2) * 255 + 0.5);
				int b = static_cast<int>(std::pow(std::min(pixels_[i].z_, 1.0), 1/2.2) * 255 + 0.5);
				out << r << " " << g << " " << b << " ";
			}
			out.close();
			return file;
		}
};

} // namespace Zebra

#endif /* _INTEGRATOR_HPP_ */