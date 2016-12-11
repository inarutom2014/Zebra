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
			std::cerr << "\ntime:  " << t << "  s\n";
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
				int r = static_cast<int>(std::min(pixels_[i].x_, 1.0) * 255);
				int g = static_cast<int>(std::min(pixels_[i].y_, 1.0) * 255);
				int b = static_cast<int>(std::min(pixels_[i].z_, 1.0) * 255);
				out << r << " " << g << " " << b << " ";
			}
			out.close();
			return file;
		}

		// std::string save_png()
		// {
		// 	time_t t;
		// 	struct tm *tt;
		// 	time(&t);
		// 	tt = localtime(&t);
		// 	std::ostringstream os;
		// 	os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
		// 	std::string file("../image/" + os.str());
		// 	file += ".png";
		// 	std::cerr << "save to: " << file << std::endl;

		// 	FILE *fp = fopen(file.c_str(), "wb");
		// 	if (!fp) {
		// 		std::cerr << "文件打开失败 :(\n";
		// 		exit(-1);
		// 	}

		// 	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		// 	if (!png_ptr) {
		// 		std::cerr << "png格式图片保存失败 :(\n";
		// 		fclose(fp);
		// 		exit(-1);
		// 	}
		// 	png_infop info_ptr = png_create_info_struct(png_ptr);
		// 	if (!info_ptr) {
		// 		png_destroy_write_struct(&png_ptr, (png_infopp) 0);
		// 		std::cerr << "png格式图片保存失败 :(\n";
		// 		fclose(fp);
		// 		exit(-1);
		// 	}

		// 	if (setjmp(png_jmpbuf(png_ptr))) {
		// 		png_destroy_write_struct(&png_ptr, &info_ptr);
		// 		std::cerr << "png格式图片保存失败 :(\n";
		// 		fclose(fp);
		// 		exit(-1);
		// 	}

		// 	png_init_io(png_ptr, fp);
		// 	int width = pixel_bound_.x_, height = pixel_bound_.y_;
		// 	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
		// 	  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		// 	png_write_info(png_ptr, info_ptr);
		// 	png_set_flush(png_ptr, 10);

		// 	unsigned char pic[height * width * 4];
		// 	uint32_t canvas[width * height];
		// 	for (int i = 0, end = width * height; i < end; ++i) {
		// 		canvas[i] = 0;
		// 		canvas[i] |= 0xFF << 24;
		// 		canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].z_, 1.0) * 255.0) << 16;
		// 		canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].y_, 1.0) * 255.0) << 8;
		// 		canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].x_, 1.0) * 255.0);
		// 	}

		// 	memcpy(pic, static_cast<void *>(canvas), height * width * 4);
		// 	png_bytep row_pointers[height];

		// 	for (int y = 0; y < height; ++y)
		// 		row_pointers[y] = static_cast<png_byte *>(pic + y * width * 4);
		// 	png_write_image(png_ptr, row_pointers);
		// 	png_write_end(png_ptr, info_ptr);

		// 	png_destroy_write_struct(&png_ptr, &info_ptr);

		// 	fclose(fp);
		// 	return file;
		// }
};

} // namespace Zebra

#endif /* _INTEGRATOR_HPP_ */