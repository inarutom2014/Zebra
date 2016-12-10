/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-06 09:53:07
**/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "bsdf.hpp"
#include "object.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "parameter.hpp"

namespace Zebra {

class Parser
{
	public:

		Parser(const char *file):line_(1) {
			std::ifstream in(file);
			if (!in.is_open()) {
				std::cerr << "file " << file << " not existed :(\n";
				exit(-1);
			}
			std::string str;
			for (; !in.eof() || str.size(); ++line_, str.clear(), std::getline(in, str)) {
				if (!str.size() || str.find("#") != std::string::npos)
					continue;
				parameter_ = Parameter(str, line_);
				std::string s(parameter_.FindString());
				if (s == "Object") objects_.push_back(FindObject());
				else if (s == "Light") lights_.push_back(FindLight());
				else {
					std::cerr << "line: " << line_ << ": syntax error :(\n";
					exit(-1);
				}
				assert(parameter_.eof());
			}
		}

		Object* FindObject() {
			std::string s(parameter_.FindString());
			const BSDF *bsdf = nullptr;
			if (s == "Diffuse") bsdf = NewDiffuseBSDF(parameter_);
			else if (s == "Reflect") bsdf = NewReflectBSDF(parameter_);
			else if (s == "Refract") bsdf = NewRefractBSDF(parameter_);
			else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
			std::string o(parameter_.FindString());
			if (o == "Sphere")
				return NewSphere(parameter_, bsdf);
			else if (o == "Plane")
				return NewPlane(parameter_, bsdf);
			else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
		}

		Light* FindLight() {
			std::string s(parameter_.FindString());
			if (s == "Point") return NewPointLight(parameter_);
			else if (s == "Directional") return NewDirectionalLight(parameter_);
			std::cerr << "line: " << line_ << ": syntax error :(\n";
			exit(-1);
		}

		Scene GetScene() const {
			assert(objects_.size() && lights_.size());
			return Scene(objects_, lights_);
		}

		~Parser() {
			std::for_each(lights_.begin(), lights_.end(), [](Light *light) {
				delete light;
			});
			std::for_each(objects_.begin(), objects_.end(), [](Object *object) {
				delete object;
			});
		}

	private:
		int       line_;
		Parameter parameter_;
		std::vector<Light *>  lights_;
		std::vector<Object *> objects_;
};

} // namespace Zebra

#endif /* _PARSER_HPP_ */