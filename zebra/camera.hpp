/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-12 23:11:35
**/

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "vector.hpp"
#include "point.hpp"
#include "bsdf.hpp"

namespace Zebra {

class Camera
{
	public:
		Camera(const Point2<int> &resolution = Point2<int>(512, 512))
		:resolution_(resolution) { }

		Vector RasterToWorld(const Point2<double> &p) const {
			static const double cx = (double)resolution_.x_ / resolution_.y_;
			static const double cy = 1;
			return Normalize(Vector(cx * ((p.x_ / resolution_.x_) - 0.5),
                              cy * (0.5 - (p.y_ / resolution_.y_)),
                              -1));
		}

		Point2<int> WorldToRaster(const Vector &v) const {
			if (v.z_ <= 0) return Point2<int>(-1, -1);
			return Point2<int>((0.5 - v.x_) * resolution_.x_,
                         (0.5 + v.y_) * resolution_.y_);
		}

		Vector DirectionToCamera(const Point &p) const {
			return Normalize(Point(0) - p);
		}

		int RasterToIndex(const Point2<int> &raster) const {
			return raster.y_ * resolution_.x_ + raster.x_;
		}

		bool RasterIsValid(const Point2<int> &p) const {
			return p.x_ >= 0 && p.y_ >= 0 && p.x_ < resolution_.x_ && p.y_ < resolution_.y_;
		}

		const Point2<int> resolution_;
};

} // namespace Zebra

#endif /* _CAMERA_HPP_ */