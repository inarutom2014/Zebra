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
#include "matrix.hpp"
#include "camera.hpp"
#include "bsdf.hpp"

namespace Zebra {

class Camera
{
	public:
		Camera(const Point2i &resolution = Point2i(512, 384))
		:resolution_(resolution) {

			auto f = 0.5135;
			auto x = resolution_.x_, y = resolution_.y_;
			auto raster_to_screen = Scale(f, -f, 1) * Transform(-.5, -.5, 0) * Scale(1./x, 1./y, 1);
			auto perspective  = Perspective(f, 1, 10000);

			raster_to_world_ = Inverse(perspective) * raster_to_screen;
			world_to_raster_ = Inverse(raster_to_world_);
		}

		Vector RasterToWorld(const Point2 &p) const {
			return Normalize(Vector(raster_to_world_(Point(p.x_, p.y_, 0))));
		}

		Point2i WorldToRaster(const Vector &v) const {
			Point p(world_to_raster_(Point(v)));
			return Point2i(p.x_, p.y_);
		}

		Spectrum SampleWi(const Point &p, Vector &wi, Point2i &raster, double &pdf,
			double &distance) const {
			Vector dir = Point(0) - p;
			distance = dir.Length();
			wi = dir / distance;
			double cosi = Dot(Vector(0, 0, -1), wi);
			pdf = distance * distance / std::abs(cosi);
			raster = WorldToRaster(wi);
			double tmp = cosi * cosi;
			return Spectrum(1.0 / (tmp * tmp));
		}

		int RasterToIndex(const Point2i &raster) const {
			return raster.y_ * resolution_.x_ + raster.x_;
		}

		bool RasterIsValid(const Point2i &p) const {
			return p.x_ >= 0 && p.y_ >= 0 && p.x_ < resolution_.x_ && p.y_ < resolution_.y_;
		}

		const Point2i resolution_;

	private:
		Matrix world_to_raster_;
		Matrix raster_to_world_;
};

} // namespace Zebra

#endif /* _CAMERA_HPP_ */