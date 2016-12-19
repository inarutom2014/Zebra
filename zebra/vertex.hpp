/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-19 10:11:46
**/

#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "vector.hpp"
#include "point.hpp"

namespace Zebra {

class Vertex
{
	public:
		Vertex() { }

		Vertex(const Point &position, const Vector &normal):position_(position), normal_(normal) { }

		Point  position_;
		Vector normal_;
};

} // namespace Zebra

#endif /* _VERTEX_HPP_ */