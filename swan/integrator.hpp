/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Swan
 *    > Created Time:  2016-12-05 22:36:27
**/

#ifndef _INTEGRATOR_HPP_
#define _INTEGRATOR_HPP_

#include <random>
#include <ctime>

#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"

namespace Swan {

class Integrator
{
	public:
		Integrator(int samples, const Point2<int> &pixel_bound)
		:samples_(samples), pixel_bound_(pixel_bound),
     pixels_(new Vector[pixel_bound_.x_*pixel_bound_.y_]) { }

		void Render() {
			std::default_random_engine generator(time(0));
			std::uniform_real_distribution<double> distribution(0, 1);
			Spectrum L;
			for (int x = 0; x != pixel_bound_.x_; ++x) {
				for (int y = 0; y != pixel_bound_.y_; ++y, L = Vector()) {
					for (int n = 0; n != samples_; ++n) {
						double dx = distribution(generator), dy = distribution(generator);
						Ray ray(Point(), Vector((dx + x) / pixel_bound_.x_,
                                    (dy + y) / pixel_bound_.y_,
                                    -1));
						// L += Trace(ray, scene);
					}
					pixels_[x * pixel_bound_.y_ + y] = L / samples_;
				}
			}
			// SaveImage();
		}

		virtual void Trace() const = 0;

		virtual ~Integrator() { delete [] pixels_; }

	protected:

		// SaveImage();

		const int         samples_;
		const Point2<int> pixel_bound_;
		Vector           *pixels_;
};

} // namespace Swan

#endif /* _INTEGRATOR_HPP_ */