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
#include <png.h>

#include "vector.hpp"
#include "camera.hpp"

namespace Zebra {

class Integrator
{
	public:
		Integrator(const int iterations, const Scene &scene)
		:iterations_(iterations), camera_(Camera()), scene_(scene),
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

		bool save_png()
		{
			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
			std::string file("../image/" + os.str());
			file += ".png";
			std::cerr << "save to: " << file << std::endl;

			FILE *fp = fopen(file.c_str(), "wb");
			if (!fp) {
				std::cerr << "文件打开失败 :(\n";
				return false;
			}

			png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
			if (!png_ptr) {
				std::cerr << "png格式图片保存失败 :(\n";
				fclose(fp);
				return false;
			}
			png_infop info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) {
				png_destroy_write_struct(&png_ptr, (png_infopp) 0);
				std::cerr << "png格式图片保存失败 :(\n";
				fclose(fp);
				return false;
			}

			if (setjmp(png_jmpbuf(png_ptr))) {
				png_destroy_write_struct(&png_ptr, &info_ptr);
				std::cerr << "png格式图片保存失败 :(\n";
				fclose(fp);
				return false;
			}

			png_init_io(png_ptr, fp);
			int width = camera_.resolution_.x_, height = camera_.resolution_.y_;

			png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
			  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			png_write_info(png_ptr, info_ptr);
			png_set_flush(png_ptr, 10);

			unsigned char pic[height * width * 4];
			uint32_t canvas[width * height];
			for (int i = 0, end = width * height; i < end; ++i) {
				canvas[i] = 0;
				canvas[i] |= 0xFF << 24;
				canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].z_, 1.0) * 255.0) << 16;
				canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].y_, 1.0) * 255.0) << 8;
				canvas[i] |= static_cast<uint8_t>(std::min(pixels_[i].x_, 1.0) * 255.0);
			}

			memcpy(pic, static_cast<void *>(canvas), height * width * 4);
			png_bytep row_pointers[height];

			for (int y = 0; y < height; ++y)
				row_pointers[y] = static_cast<png_byte *>(pic + y * width * 4);
			png_write_image(png_ptr, row_pointers);
			png_write_end(png_ptr, info_ptr);

			png_destroy_write_struct(&png_ptr, &info_ptr);

			fclose(fp);
			return true;
		}

		const int         iterations_;
		const Camera      camera_;
		const Scene       scene_;
		Vector           *pixels_;

		std::default_random_engine generator_;
		std::uniform_real_distribution<double> distribution_;
};

} // namespace Zebra

#endif /* _INTEGRATOR_HPP_ */