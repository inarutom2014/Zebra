/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-02 09:33:46
**/

#ifndef _KERNAL_HPP_
#define _KERNAL_HPP_

#include "matrix.hpp"

namespace Bunny {

class Kernal
{
	public:
		enum Type { kLinear };

		Kernal(Type type = kLinear):type_(type) {
			switch (type_): {
				case kLinear:
					kernal_ = Linear;
			}
		}

		template<typename T, typename U>
		T operator()(const Vector<T> &lhs, const Vector<U> &rhs) const {
			return std::move(lhs.Dot(rhs));
		}

	private:

		Type type_;
};

} // namespace Bunny

#endif /* _KERNAL_HPP_ */