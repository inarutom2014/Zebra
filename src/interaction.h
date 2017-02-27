/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Zebra
 *    > Created Time:  2016-12-31 23:13:47
**/

#ifndef _INTERACTION_H_
#define _INTERACTION_H_

#include "vector.h"

namespace Zebra {

class Object;

class Interaction
{
	public:
		Interaction() { }
		Interaction(const Point &p, const Vector &n, const Object *o):p_(p), n_(n), o_(o) { }

		Point         p_;
		Vector        n_;
		const Object *o_;
};

} // namespace Zebra

#endif /* _INTERACTION_H_ */