/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-05 22:36:27
**/

#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_

#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <png.h>

#include "vector.hpp"
#include "camera.hpp"
#include "rng.hpp"

namespace Zebra {

class Integrator
{
	public:
		Integrator(const int iterations, const Scene &scene)
		:iterations_(iterations), camera_(Camera()), scene_(scene),
		 pixels_(new Vector[camera_.resolution_.x_ * camera_.resolution_.y_]) { }

		virtual std::string Render() = 0;

		virtual ~Integrator() { delete [] pixels_; }

	protected:

		std::string WriteBMP() const {
      struct BmpHeader {
        uint   mFileSize;
        uint   mReserved01;
        uint   mDataOffset;

        uint   mHeaderSize;
        int    mWidth;
        int    mHeight;

        short  mColorPlates;
        short  mBitsPerPixel;
        uint   mCompression;
        uint   mImageSize;
        uint   mHorizRes;
        uint   mVertRes;
        uint   mPaletteColors;
        uint   mImportantColors;
      };

			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
      os << "_" << iterations_;
			std::string file("./1/" + os.str());
			file += ".bmp";
			fprintf(stderr, "save to: %s.bmp\n", os.str().c_str());

      int X = camera_.resolution_.x_, Y = camera_.resolution_.y_;

      std::ofstream bmp(file, std::ios::binary);
      assert(bmp.is_open());
      BmpHeader header;
      bmp.write("BM", 2);
      header.mFileSize   = uint(sizeof(BmpHeader) + 2) + X * Y * 3;
      header.mReserved01 = 0;
      header.mDataOffset = uint(sizeof(BmpHeader) + 2);
      header.mHeaderSize = 40;
      header.mWidth      = X;
      header.mHeight     = Y;
      header.mColorPlates     = 1;
      header.mBitsPerPixel    = 24;
      header.mCompression     = 0;
      header.mImageSize       = X * Y * 3;
      header.mHorizRes        = 2953;
      header.mVertRes         = 2953;
      header.mPaletteColors   = 0;
      header.mImportantColors = 0;

      bmp.write((char *)&header, sizeof(header));

      typedef unsigned char byte;

      for (int y = 0; y < Y; ++y) {
        for (int x = 0; x < X; ++x) {
          const Vector &rgb = pixels_[x + (Y - y - 1) * X];
          double tmp[3];
          tmp[0] = rgb.z_ * 255 + 0.5;
          tmp[1] = rgb.y_ * 255 + 0.5;
          tmp[2] = rgb.x_ * 255 + 0.5;

          byte clr[3];
          clr[0] = byte(std::min(255.0, std::max(0.0, tmp[0])));
          clr[1] = byte(std::min(255.0, std::max(0.0, tmp[1])));
          clr[2] = byte(std::min(255.0, std::max(0.0, tmp[2])));

          bmp.write((char *)&clr, sizeof(clr));
        }
      }
      return file;
		}

    std::string WritePNG() {
      time_t t;
      struct tm *tt;
      time(&t);
      tt = localtime(&t);
      std::ostringstream os;
      os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
      os << "_" << iterations_;
      std::string file("./1/" + os.str());
      file += ".png";
      fprintf(stderr, "save to: %s.png\n", os.str().c_str());

      FILE *fp = fopen(file.c_str(), "wb");
      assert(fp);

      png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
      png_infop info_ptr = png_create_info_struct(png_ptr);

      png_init_io(png_ptr, fp);

      int width = camera_.resolution_.x_, height = camera_.resolution_.y_;

      png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
      png_write_info(png_ptr, info_ptr);
      png_set_flush(png_ptr, 10);

      unsigned char pic[height * width * 4];
      uint32_t canvas[width * height];
      for (int i = 0; i < width * height; ++i) {
        canvas[i] = 0;
        canvas[i] |= 0xFF << 24;
        canvas[i] |= (uint8_t)(std::min(1.0, std::max(0.0, pixels_[i].z_)) * 255.0 + 0.5) << 16;
        canvas[i] |= (uint8_t)(std::min(1.0, std::max(0.0, pixels_[i].y_)) * 255.0 + 0.5) << 8;
        canvas[i] |= (uint8_t)(std::min(1.0, std::max(0.0, pixels_[i].x_)) * 255.0 + 0.5);
      }

      memcpy(pic, static_cast<void *>(canvas), height * width * 4);
      png_bytep row_pointers[height] = {0};

      for (int y = 0; y < height; ++y)
        row_pointers[y] = static_cast<png_byte *>(pic + y * width * 4);
      png_write_image(png_ptr, row_pointers);
      png_write_end(png_ptr, info_ptr);

      png_destroy_write_struct(&png_ptr, &info_ptr);
      fclose(fp);

      return file;
    }

		const int         iterations_;
		const Camera      camera_;
		const Scene       scene_;
		Vector           *pixels_;
		Rng               rng_;
};

} // namespace Zebra

#endif /* _INTEGRATOR_HPP_ */