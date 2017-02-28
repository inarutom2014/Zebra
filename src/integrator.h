/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Zebra
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

		std::string WriteBMP(int samples) const {
      struct BmpHeader {
        uint   mFileSize;        // Size of file in bytes
        uint   mReserved01;      // 2x 2 reserved bytes
        uint   mDataOffset;      // Offset in bytes where data can be found (54)

        uint   mHeaderSize;      // 40B
        int    mWidth;           // Width in pixels
        int    mHeight;          // Height in pixels

        short  mColorPlates;     // Must be 1
        short  mBitsPerPixel;    // We use 24bpp
        uint   mCompression;     // We use BI_RGB ~ 0, uncompressed
        uint   mImageSize;       // mWidth x mHeight x 3B
        uint   mHorizRes;        // Pixels per meter (75dpi ~ 2953ppm)
        uint   mVertRes;         // Pixels per meter (75dpi ~ 2953ppm)
        uint   mPaletteColors;   // Not using palette - 0
        uint   mImportantColors; // 0 - all are important
      };

			time_t t;
			struct tm *tt;
			time(&t);
			tt = localtime(&t);
			std::ostringstream os;
			os << tt->tm_hour << "-" << tt->tm_min << "-" << tt->tm_sec;
      os << "_" << (samples * 4);
			std::string file("./test/" + os.str());
			file += ".bmp";
			fprintf(stderr, "save to: %s.bmp\n", os.str().c_str());

      std::ofstream bmp(file, std::ios::binary);
      assert(bmp.is_open());
      BmpHeader header;
      bmp.write("BM", 2);
      header.mFileSize   = uint(sizeof(BmpHeader) + 2) + camera_.x_ * camera_.y_ * 3;
      header.mReserved01 = 0;
      header.mDataOffset = uint(sizeof(BmpHeader) + 2);
      header.mHeaderSize = 40;
      header.mWidth      = camera_.x_;
      header.mHeight     = camera_.y_;
      header.mColorPlates     = 1;
      header.mBitsPerPixel    = 24;
      header.mCompression     = 0;
      header.mImageSize       = camera_.x_ * camera_.y_ * 3;
      header.mHorizRes        = 2953;
      header.mVertRes         = 2953;
      header.mPaletteColors   = 0;
      header.mImportantColors = 0;

      bmp.write((char *)&header, sizeof(header));

      typedef unsigned char byte;

      for (int y = 0; y < camera_.y_; ++y) {
        for (int x = 0; x < camera_.x_; ++x) {
          Vector &rgb = pixels_[x + (camera_.y_ - y - 1) * camera_.x_];
          double tmp[3];
          // tmp[0] = std::pow(rgb.z_, 1.0/2.2) * 255 + 0.5;
          // tmp[1] = std::pow(rgb.y_, 1.0/2.2) * 255 + 0.5;
          // tmp[2] = std::pow(rgb.x_, 1.0/2.2) * 255 + 0.5;
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

	protected:
		RNG     rng_;
		Camera  camera_;
		Vector *pixels_;
};

} // namespace Zebra

#endif /* _INTEGRATOR_H_ */