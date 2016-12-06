/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Bunny
 *    > Created Time:  2016-12-06 09:47:22
**/

#ifndef _SCENE_HPP_
#define _SCENE_HPP_

namespace Swan {

class Scene
{
	public:
		Scene(const std::vector<Object> &objects, const std::vector<Light> &lights)
		:objects_(objects), lights_(lights) { }

		const std::vector<Object *>& Object() const {
			return objects_;
		}

		const std::vector<Light *>& Light() const {
			return lights_;
		}

	private:
		const std::vector<Object *> objects_;
		const std::vector<Light *>  lights_;
};

} // namespace Swan

#endif /* _SCENE_HPP_ */