/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-31 23:40:47
**/

#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>

#include "vector.h"
#include "object.h"

namespace Zebra {

class Parser
{
	public:
		Parser(const char *file) {
			std::ifstream in(file);
			assert(in.is_open());
			std::string str;
			for (; !in.eof() || str.size(); std::getline(in, str)) {
				if (!str.size() || str.find("#") != std::string::npos)
					continue;
				std::istringstream s(str);
				std::string tmp;
				s >> tmp;
				assert(tmp == "Sphere");
				s >> tmp;
				BSDF t = Diffuse;
				if (tmp == "Diffuse") t = Diffuse;
				if (tmp == "Reflect") t = Reflect;
				if (tmp == "Refract") t = Refract;
				s >> tmp;
				assert(tmp == "Position");
				Vector p;
				s >> p.x_ >> p.y_ >> p.z_;
				assert(!s.fail());
				s >> tmp;
				assert(tmp == "Radius");
				double r;
				s >> r;
				assert(!s.fail());
				s >> tmp;
				assert(tmp == "Spectrum");
				Vector e;
				s >> e.x_ >> e.y_ >> e.z_;
				assert(!s.fail());
				s >> tmp;
				assert(tmp == "Spectrum");
				Vector c;
				s >> c.x_ >> c.y_ >> c.z_;
				assert(!s.fail());
				scene_.push_back(new Sphere(t, p, r, e, c));
			}
		}

		const std::vector<Object *>& Scene() const { return scene_; }

		~Parser() {
			for (size_t i = 0; i != scene_.size(); ++i)
				delete scene_[i];
		}

	private:
		std::vector<Object *> scene_;
};

} // namespace Zebra

#endif /* _PARSER_H_ */